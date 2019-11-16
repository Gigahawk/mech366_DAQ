#include "webserver.h"

ESP8266WebServer server(80);       // create a web server on port 80
File fsUploadFile;

void handleNotFound(){ // if the requested file or page doesn't exist, return a 404 not found error
    if(!handleFileRead(server.uri())){          // check if the file exists in the flash memory (SPIFFS), if so, send it
        server.send(404, "text/plain", "404: File Not Found");
    }
}

//oid handleLogin() {                         // If a POST request is made to URI /login
//  if( ! server.hasArg("username") || ! server.hasArg("password") 
//      || server.arg("username") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
//    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
//    return;
//  }
//  if(server.arg("username") == "John Doe" && server.arg("password") == "password123") { // If both the username and the password are correct
//    server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>Login successful</p>");
//  } else {                                                                              // Username and password don't match
//    server.send(401, "text/plain", "401: Unauthorized");
//  }
//}

void handleFileDelete() {
    if(!server.hasArg("path") || server.arg("path") == NULL) {
        Serial.println("handleFileDelete: no path provided");
    } else {
        String path = server.arg("path");
        Serial.println("handleFileDelete: " + path);
        if (!path.endsWith("/") 
                && !path.endsWith("index.html") 
                && !path.endsWith("WebSocket.js")) 
            SPIFFS.remove(path);
    }

    server.sendHeader("Location","/index.html");      // Redirect the client to the success page
    server.send(303);
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
    String contentType = getContentType(path);             // Get the MIME type
    String pathWithGz = path + ".gz";
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
        if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
            path += ".gz";                                         // Use the compressed verion
        File file = SPIFFS.open(path, "r");                    // Open the file
        size_t sent = server.streamFile(file, contentType);    // Send it to the client
        file.close();                                          // Close the file again
        Serial.println(String("\tSent file: ") + path);
        return true;
    }
    Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
    return false;
}

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

void handleFileUpload(){ // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location","/index.html");      // Redirect the client to the success page
      server.send(303);
    } else {
      Serial.println("File upload failed");
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}
