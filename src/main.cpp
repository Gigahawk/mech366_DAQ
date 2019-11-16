#include<Arduino.h>

#include "config.h"
#include "helpers.h"
#include "datastore.h"
#include "motordriver.h"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);        // Start the Serial communication to send messages to the computer
    delay(10);
    Serial.println("\r\n");

    startAccel();

    startWiFi();

    startSPIFFS();
    
    startWebSocket();
    
    startServer();

    startMotor();
    startAccel();
}

void loop() {
    webSocket.loop();                           // constantly check for websocket events
    server.handleClient();                      // run the server
    motor_loop();
    accel_loop();
}


