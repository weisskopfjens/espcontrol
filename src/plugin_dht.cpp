
#include "plugin_dht.h"

extern plugin_DHT dhtPlugin;
extern LuaWrapper lua;

int lua_getDHTvalues (lua_State *L) {
  float humidity = dhtPlugin.dht.getHumidity();
  float temperature = dhtPlugin.dht.getTemperature();
  lua_pushnumber(L, temperature);
  lua_pushnumber(L, humidity);
  return 2;
}

int lua_initDHT (lua_State *L) {
  uint8_t pin = luaL_checkinteger(L, 1);
  dhtPlugin.dht.setup(pin);
  lua.Lua_register(F("getDHTvalues"), (const lua_CFunction) &lua_getDHTvalues);
  return 0;
}


plugin_DHT::plugin_DHT(void) {

}

void plugin_DHT::setup(void) {
  lua.Lua_register(F("initDHT"), (const lua_CFunction) &lua_initDHT);
  Serial.println(F("INFO: setup plugin_DHT"));
}

void plugin_DHT::loop(void) {

}

void plugin_DHT::json (AsyncWebServerRequest *request,AsyncResponseStream *response) {
  if (request->hasParam("DHT")) {
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    response->print(F("{\"DHT\":"));
  	response->print(F("{"));
  	response->print(F("\"Temperature\":"));
    response->print((String)temperature);
  	response->print(F(",\"Humidity\":"));
    response->print((String)humidity);
	  response->print(F("}}"));
  }
}
