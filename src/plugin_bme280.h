#ifndef PLUGIN_BME280_H_
#define PLUGIN_BME280_H_

#include <LuaWrapper.h>
#include <BME280I2C.h>
#include <Wire.h>
#include "plugin.h"

// I2C pin 4(SDA) 5(SCL)
// Wire.begin(sda,scl);
//int lua_getBME280values (lua_State *L);

class plugin_BME280 : public Plugin {
  public:
    plugin_BME280(void);
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    BME280I2C bmesensor;
  private:

};



#endif
