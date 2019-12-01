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
    esc.writeMicroseconds(ESC_MIN_PULSE);
    Serial.println("Motor driver started.");
}

void startAccel() {
    accel_need_save = false;
    Serial.println("Initializing I2C devices...");
    Wire.begin();

    accel.initialize();
    pinMode(ACCEL_INT, INPUT);
    detachInterrupt(digitalPinToInterrupt(ACCEL_INT));

    Serial.println("Testing device connections...");
    if(accel.testConnection()) {
        Serial.println("MPU6050 connection successful");
        imu_connected = true;
    } else {
        Serial.println("MPU6050 connection failed");
        imu_connected = false;
    }

    Serial.println("Updating internal sensor offsets...");
    accel.setXAccelOffset(ACCEL_X_OFST);
    accel.setYAccelOffset(ACCEL_Y_OFST);
    accel.setZAccelOffset(ACCEL_Z_OFST);
    accel.setXGyroOffset(GYRO_X_OFST);
    accel.setYGyroOffset(GYRO_Y_OFST);
    accel.setZGyroOffset(GYRO_Z_OFST);
    Serial.print(accel.getXAccelOffset()); Serial.print("\t");
    Serial.print(accel.getYAccelOffset()); Serial.print("\t");
    Serial.print(accel.getZAccelOffset());
    Serial.print("\n");

    accel.setRate((8000/ACCEL_RATE) - 1);
    Serial.print("Current rate: ");
    Serial.println(8000/(1 + accel.getRate()));

    accel.setInterruptMode(MPU6050_INTMODE_ACTIVELOW);
    //accel.setInterruptDrive(MPU6050_INTDRV_OPENDRAIN);
    accel.setInterruptLatch(MPU6050_INTLATCH_WAITCLEAR);
    accel.setInterruptLatchClear(MPU6050_INTCLEAR_ANYREAD);
    Serial.println("Getting interrupt pin status: ");
    Serial.print("Mode: ");
    Serial.println(accel.getInterruptMode() ? "Active Low" : "Active High");
    Serial.print("Drive: ");
    Serial.println(accel.getInterruptDrive() ? "Open drain" : "Push pull");
    Serial.print("Latch: ");
    Serial.println(accel.getInterruptLatch() ? "Wait clear" : "50us pulse");
    Serial.print("Clear: ");
    Serial.println(accel.getInterruptLatchClear() ? "Any read" : "Status read");

    Serial.println("Getting interrupts: ");
    //accel.setIntEnabled(MPU6050_INTERRUPT_DATA_RDY_BIT);
    accel.setIntDataReadyEnabled(true);
    Serial.print("Freefall: ");
    Serial.println(accel.getIntFreefallEnabled());
    Serial.print("Motion: ");
    Serial.println(accel.getIntMotionEnabled());
    Serial.print("Zero Motion: ");
    Serial.println(accel.getIntZeroMotionEnabled());
    Serial.print("FIFO Overflow: ");
    Serial.println(accel.getIntFIFOBufferOverflowEnabled());
    Serial.print("I2C Master: ");
    Serial.println(accel.getIntI2CMasterEnabled());
    Serial.print("Data Ready: ");
    Serial.println(accel.getIntDataReadyEnabled());

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
