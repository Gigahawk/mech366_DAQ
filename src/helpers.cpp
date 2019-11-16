#include "helpers.h"

String formatBytes(size_t bytes);

void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
    WiFi.softAP(AP_SSID, AP_PASSWORD);             // Start the access point
    Serial.print("Access Point \"");
    Serial.print(AP_SSID);
    Serial.println("\" started\r\n");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    Serial.println("\r\n");
}

void startMotor() {
    motor_running = false;
    esc.attach(ESC_PIN);
    Serial.println("Motor driver started.");
}

void startAccel() {
    accel_need_save = false;
    accel.powerOn();
    accel.setRate(ACCEL_RATE);
    // Interrupt driven data collection ftw
    accel.setInterrupt(ADXL345_INT_DATA_READY_BIT, 1);
    Serial.println("Accelerometer driver started.");
}

void startWebSocket() { // Start a WebSocket server
    webSocket.begin();                          // start the websocket server
    webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
    Serial.println("WebSocket server started.");
}

void startServer() { // Start a HTTP server with a file read handler and an upload handler
    server.on("/", HTTP_POST,                       // if the client posts to the upload page
            [](){},
            handleFileUpload                                    // Receive and save the file
            );
    server.on("/index.html", HTTP_POST,                       // if the client posts to the upload page
            [](){},
            handleFileUpload                                    // Receive and save the file
            );
    server.on("/delete", HTTP_POST, handleFileDelete);
    server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
    // and check if the file exists

    server.begin();                             // start the HTTP server
    Serial.println("HTTP server started.");
}

void startSPIFFS() { // Start the SPIFFS and list all contents
    SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)
    Serial.println("SPIFFS started. Contents:");
    {
        Dir dir = SPIFFS.openDir("/");
        while (dir.next()) {                      // List the file system contents
            String fileName = dir.fileName();
            size_t fileSize = dir.fileSize();
            Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
        }
        Serial.printf("\n");
    }
}

String formatBytes(size_t bytes) { // convert sizes in bytes to KB and MB
    if (bytes < 1024) {
        return String(bytes) + "B";
    } else if (bytes < (1024 * 1024)) {
        return String(bytes / 1024.0) + "KB";
    } else if (bytes < (1024 * 1024 * 1024)) {
        return String(bytes / 1024.0 / 1024.0) + "MB";
    }
}
