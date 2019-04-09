#ifndef PLUGIN_DHT_H_
#define PLUGIN_DHT_H_

#include <LuaWrapper.h>
#include "DHT.h"
#include "plugin.h"

class plugin_DHT : public Plugin {
  public:
    plugin_DHT(void);
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    DHT dht;
  private:

};

#endif
