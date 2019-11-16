#include "websockets.h"

WebSocketsServer webSocket(81); // create a websocket server on port 81
String getFileList();

void handleCommand(uint8_t client_id, char* cmd);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("[%u] get Text: %s\n", num, payload);
      handleCommand(num, (char*) payload);
      break;
  }
}

void handleCommand(uint8_t client_id, char* command) {
    String resp;
    char *cmd, *arg;
    cmd = strtok(command, " ");
    arg = strtok(NULL, " ");
    Serial.print("cmd: ");
    Serial.println(cmd);
    Serial.print("arg: ");
    Serial.println(arg);

    if(strcmp(cmd, "ls") == 0){
        Serial.println("Sending file list");
        resp = getFileList();
        webSocket.sendTXT(client_id, resp);
    } else if(strcmp(cmd, "load") == 0){
        if(arg){
            Serial.print("Loading file ");
            Serial.println(arg);
            fillBuffer(arg);
        }
        resp = String(":load\r\n") + loaded_file;
        webSocket.sendTXT(client_id, resp);
    } else if(strcmp(cmd, "run") == 0){
        Serial.println("Starting run");
        start_data();
        start_motor();
    }
    else {
        Serial.printf("Unknown command %s\n", cmd);
    }
}

String getFileList() {
    String str = String(":ls");
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {                      // List the file system contents
        String fileName = dir.fileName();
        str += "\r\n";
        str += fileName;
    }
    return str;
}

