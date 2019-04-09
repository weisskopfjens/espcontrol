#ifndef PLUGIN_RC_H_
#define PLUGIN_RC_H_

#include <IRremoteESP8266.h>
#include <RCSwitch.h>
#include <LuaWrapper.h>
#include "plugin.h"

class plugin_RC : public Plugin {
  public:
    plugin_RC(void);
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    RCSwitch rcSwitch;
    int getValue(void);
    int _rcvalue;
  private:

};



#endif
