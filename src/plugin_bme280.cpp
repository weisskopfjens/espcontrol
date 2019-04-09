
#include "plugin_bme280.h"

extern plugin_BME280 bme280Plugin;
extern LuaWrapper lua;

int lua_getBME280values (lua_State *L) {
  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  bme280Plugin.bmesensor.read(pres, temp, hum, tempUnit, presUnit);
  lua_pushnumber(L, temp);
  lua_pushnumber(L, hum);
  lua_pushnumber(L, pres);
  return 3;
}

// I2C pin 4(SDA) 5(SCL)
int lua_initBME280 (lua_State *L) {
  uint8_t sda = luaL_checkinteger(L, 1);
  uint8_t scl = luaL_checkinteger(L, 2);
  Wire.begin(sda,scl);
  if(!bme280Plugin.bmesensor.begin()) {
    Serial.println(F("ERROR: Could not find BME280 sensor!"));
  }
  switch(bme280Plugin.bmesensor.chipModel()) {
     case BME280::ChipModel_BME280:
       Serial.println(F("INFO: Found BME280 sensor! Success."));
       break;
     case BME280::ChipModel_BMP280:
       Serial.println(F("INFO: Found BMP280 sensor! No Humidity available."));
       break;
     default:
       Serial.println(F("ERROR: Found UNKNOWN sensor! Error!"));
  }
  lua.Lua_register(F("getBME280values"), (const lua_CFunction) &lua_getBME280values);
  return 0;
}

plugin_BME280::plugin_BME280(void) {

}

void plugin_BME280::setup(void) {

  lua.Lua_register(F("initBME280"), (const lua_CFunction) &lua_initBME280);
  Serial.println(F("INFO: setup plugin_BME280"));
}

void plugin_BME280::loop(void) {

}

void plugin_BME280::json (AsyncWebServerRequest *request,AsyncResponseStream *response) {
  if (request->hasParam("bme280")) {
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme280Plugin.bmesensor.read(pres, temp, hum, tempUnit, presUnit);
    response->print(F("{\"bme280\":"));
  	response->print(F("{"));
  	response->print(F("\"Temperature\":"));
    response->print((String)temp);
  	response->print(F(",\"Humidity\":"));
    response->print((String)hum);
  	response->print(F(",\"Pressure\":"));
    response->print((String)pres);
	  response->print(F("}}"));
  }
}
