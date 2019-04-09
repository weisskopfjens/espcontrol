#ifndef PLUGIN_ICMPPING_H_
#define PLUGIN_ICMPPING_H_
#if defined(ESP8266)

#include <LuaWrapper.h>
#include "AsyncPing.h"
#include "plugin.h"

class plugin_ICMPPing : public Plugin {
  public:
    plugin_ICMPPing(void);
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    AsyncPing ICMPPing;
  private:

};

#endif
#endif
