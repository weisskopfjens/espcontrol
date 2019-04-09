#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include "core.h"

void handle_FILE_iro (AsyncWebServerRequest *request);
void handle_FILE_w3css (AsyncWebServerRequest *request);

void handleFiles (AsyncWebServerRequest *request);
void handleUpload (AsyncWebServerRequest *request);
void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void handle_FILE_filemanager (AsyncWebServerRequest *request);
void handleConfig (AsyncWebServerRequest *request);
void handleJSON (AsyncWebServerRequest *request);
void handleControl (AsyncWebServerRequest *request);
void handleUDPPacket(AsyncUDPPacket packet);

#endif
