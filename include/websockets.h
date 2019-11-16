#ifndef DAQWEBSOCKETS_H
#define DAQWEBSOCKETS_H
#include <WebSocketsServer.h>
#include <FS.h>
#include "motordriver.h"

//WebSocketsServer webSocket(81); // create a websocket server on port 81
extern WebSocketsServer webSocket;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

#endif //DAQWEBSOCKETS_H
