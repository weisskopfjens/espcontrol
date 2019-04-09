#ifndef PLUGIN_MQTT_H_
#define PLUGIN_MQTT_H_

#include <LuaWrapper.h>
#include <AsyncMqttClient.h>
#include <Ticker.h>
#include "plugin.h"

class plugin_MQTT : public Plugin {
  public:
    plugin_MQTT( void );
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    AsyncMqttClient mqttClient;
    Ticker mqttReconnectTimer;
  private:

};



#endif
