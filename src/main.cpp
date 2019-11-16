#include<Arduino.h>

#include "config.h"
#include "helpers.h"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);        // Start the Serial communication to send messages to the computer
    delay(10);
    Serial.println("\r\n");
    Serial.println((int)sizeof(uint8_t));
    Serial.println((int)sizeof(unsigned int));
    Serial.println((int)sizeof(unsigned long));

    startAccel();

    startWiFi();

    startSPIFFS();
    
    startWebSocket();
    
    startServer();
}

void loop() {
    webSocket.loop();                           // constantly check for websocket events
    server.handleClient();                      // run the server
}


