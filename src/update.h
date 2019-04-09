#ifndef UPDATE_H_
#define UPDATE_H_

#include <ESPAsyncWebServer.h>
#include "staticweb.h"
// Handle end of update process
void handleUpdate(AsyncWebServerRequest *request);

// Handle firmware upload
void handleUpdateUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

// Show update form
void handleUpdateform(AsyncWebServerRequest *request);

#endif
