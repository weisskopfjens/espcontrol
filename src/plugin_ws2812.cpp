
#include "plugin_ws2812.h"

extern plugin_WS2812 ws2812pi;
extern LuaWrapper lua;

int lua_WS2812setMode (lua_State *L) {
  uint8_t value = luaL_checkinteger(L, 1);
  ws2812pi.ws2812fx->setMode(value);
  return 0;
}

int lua_WS2812setSpeed (lua_State *L) {
  uint16_t value = luaL_checkinteger(L, 1);
  ws2812pi.ws2812fx->setSpeed(value);
  return 0;
}

int lua_WS2812chgSpeed (lua_State *L) {
  uint16_t value = luaL_checkinteger(L, 1);
  ws2812pi.ws2812fx->setSpeed(ws2812pi.ws2812fx->getSpeed()+value);
  return 0;
}

int lua_WS2812setBrightness (lua_State *L) {
  uint8_t value = luaL_checkinteger(L, 1);
  ws2812pi.ws2812fx->setBrightness(value);
  return 0;
}

int lua_WS2812chgBrightness (lua_State *L) {
  uint8_t value = luaL_checkinteger(L, 1);
  ws2812pi.ws2812fx->setBrightness(ws2812pi.ws2812fx->getBrightness()+value);
  return 0;
}

int lua_WS2812getStatus (lua_State *L) {
  lua_pushinteger(L, ws2812pi.ws2812fx->getBrightness());
  lua_pushinteger(L, ws2812pi.ws2812fx->getSpeed());
  lua_pushinteger(L, ws2812pi.ws2812fx->getMode());
  lua_pushinteger(L, ws2812pi.ws2812fx->isRunning());
  return 4;
}

int lua_WS2812control (lua_State *L) {
  uint8_t value = luaL_checkinteger(L, 1);
  switch(value) {
    case 0:
      ws2812pi.ws2812fx->stop();
    break;
    case 1:
      ws2812pi.ws2812fx->start();
    break;
    case 2:
      ws2812pi.ws2812fx->pause();
    break;
    case 3:
      ws2812pi.ws2812fx->resume();
    break;
    case 4:
      ws2812pi.ws2812fx->strip_off();
    break;
    case 5:
      ws2812pi.ws2812fx->fade_out();
    break;
  }
  return 0;
}

int lua_WS2812setColor (lua_State *L) {
  uint8_t r = luaL_checkinteger(L, 1);
  uint8_t g = luaL_checkinteger(L, 2);
  uint8_t b = luaL_checkinteger(L, 3);
  ws2812pi.ws2812fx->setColor(r,g,b);
  return 0;
}

int lua_WS2812setPixelColor (lua_State *L) {
  uint16_t p = luaL_checkinteger(L, 1);
  uint8_t r = luaL_checkinteger(L, 2);
  uint8_t g = luaL_checkinteger(L, 3);
  uint8_t b = luaL_checkinteger(L, 4);
  ws2812pi.ws2812fx->setPixelColor(p,r,g,b);
  return 0;
}

int lua_initWS2812 (lua_State *L) {
  uint8_t ledcount = luaL_checkinteger(L, 1);
  uint8_t ledpin = luaL_checkinteger(L, 2);
  ws2812pi.ws2812fx = new WS2812FX(ledcount, ledpin, NEO_GRB + NEO_KHZ800);
  ws2812pi.ws2812fx->init();
  ws2812pi.ws2812fx->setBrightness(30);
  ws2812pi.ws2812fx->setSpeed(1000);
  ws2812pi.ws2812fx->setColor(0x000000);
  ws2812pi.ws2812fx->setMode(FX_MODE_STATIC);
  ws2812pi.ws2812fx->start();
  lua.Lua_register(F("WS2812setMode"), (const lua_CFunction) &lua_WS2812setMode);
  lua.Lua_register(F("WS2812setSpeed"), (const lua_CFunction) &lua_WS2812setSpeed);
  lua.Lua_register(F("WS2812chgSpeed"), (const lua_CFunction) &lua_WS2812chgSpeed);
  lua.Lua_register(F("WS2812setBrightness"), (const lua_CFunction) &lua_WS2812setBrightness);
  lua.Lua_register(F("WS2812chgBrightness"), (const lua_CFunction) &lua_WS2812chgBrightness);
  lua.Lua_register(F("WS2812control"), (const lua_CFunction) &lua_WS2812control);
  lua.Lua_register(F("WS2812getStatus"), (const lua_CFunction) &lua_WS2812getStatus);
  lua.Lua_register(F("WS2812setColor"), (const lua_CFunction) &lua_WS2812setColor);
  lua.Lua_register(F("WS2812setPixelColor"), (const lua_CFunction) &lua_WS2812setPixelColor);
  return 0;
}


plugin_WS2812::plugin_WS2812(void) {
  ws2812fx=NULL;
}

void plugin_WS2812::setup(void) {
  lua.Lua_register(F("initWS2812"), (const lua_CFunction) &lua_initWS2812);
  Serial.println(F("INFO: setup plugin_WS2812"));
}

void plugin_WS2812::loop(void) {
  if(ws2812fx!=NULL) {
    ws2812fx->service();
  }
}

void plugin_WS2812::json(AsyncWebServerRequest *request,AsyncResponseStream *response) {
  if (request->hasParam("WS2812")) {
    response->print(F("{\"WS2812\":"));
  	response->print(F("{"));
    response->print(F("\"Speed\":"));
    response->print((String)ws2812fx->getSpeed());
  	response->print(F(",\"Brightness\":"));
    response->print((String)ws2812fx->getBrightness());
  	response->print(F(",\"Mode\":"));
    response->print((String)ws2812fx->getMode());
  	response->print(F(",\"Running\":"));
    response->print((String)ws2812fx->isRunning());
	  response->print(F("}}"));
  }
}
