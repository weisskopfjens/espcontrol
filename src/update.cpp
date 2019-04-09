#include "update.h"

// Handle end of update process
void handleUpdate(AsyncWebServerRequest *request){
  AsyncResponseStream *response = request->beginResponseStream("text/plain");
  if( Update.hasError() ) {
    response->print(F("Update FAIL. Maybe you should reboot first."));
  } else {
    response->print(F("Update OK"));
  }
  response->addHeader("Connection", "close");
  request->send(response);
}

// Handle firmware upload
void handleUpdateUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  if(!index){
    //Serial.printf("Update Start: %s\n", filename.c_str());
    Serial.printf_P(PSTR("Update Start: %s\n"), filename.c_str());
    Update.runAsync(true);
    if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
      Update.printError(Serial);
    }
  }
  if(!Update.hasError()){
    if(Update.write(data, len) != len){
      Update.printError(Serial);
    }
  }
  if(final){
    if(Update.end(true)){
      Serial.printf_P(PSTR("Update Success: %uB\n"), index+len);
    } else {
      Update.printError(Serial);
    }
  }
}

// Show update form
void handleUpdateform(AsyncWebServerRequest *request){
  AsyncResponseStream *response = request->beginResponseStream(F("text/html"));
  response->printf_P(SIDE_begin,"Update");
  response->printf_P(SIDE_update);
  response->printf_P(SIDE_end);
  request->send(response);
};
