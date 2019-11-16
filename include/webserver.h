#ifndef DAQWEBSERVER_H
#define DAQWEBSERVER_H
#include <ESP8266WebServer.h>
#include <FS.h>

extern ESP8266WebServer server;
void handleNotFound();
void handleFileUpload();
void handleFileDelete();
bool handleFileRead(String path);
String getContentType(String filename);

#endif //DAQWEBSERVER_H
