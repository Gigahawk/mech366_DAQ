#ifndef HELPERS_H
#define HELPERS_H
#include <ESP8266WiFi.h>
#include <FS.h>
#include "config.h"
#include "websockets.h"
#include "webserver.h"
#include "datastore.h"

void startWiFi();
void startWebSocket();
void startSPIFFS();
void startServer();
void startAccel();

#endif //HELPERS_H
