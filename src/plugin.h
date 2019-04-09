#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <Arduino.h>
#include <LuaWrapper.h>
#include <ESPAsyncWebServer.h>

class Plugin {
    static Plugin *headPlugin;
    Plugin *nextPlugin;
  public:
    Plugin();
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void json(AsyncWebServerRequest *request,AsyncResponseStream *response) = 0;
    static void setupAll();
    static void loopAll();
    static void jsonAll(AsyncWebServerRequest *request,AsyncResponseStream *response);
};
extern LuaWrapper lua;

#endif
