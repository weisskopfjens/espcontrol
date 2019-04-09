
#include "plugin_ssd1306.h"

extern plugin_SSD1306 ssd1306Plugin;
extern LuaWrapper lua;

int lua_ssd1306drawString (lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  const char *str = luaL_checkstring(L, 3);
  ssd1306Plugin.display->drawString(x, y, str);
  return 0;
}

int lua_ssd1306clear (lua_State *L) {
  ssd1306Plugin.display->clear();
  return 0;
}

int lua_ssd1306display (lua_State *L) {
  ssd1306Plugin.display->display();
  return 0;
}

int lua_ssd1306font (lua_State *L) {
  int f = luaL_checkinteger(L, 1);
  int a = luaL_checkinteger(L, 2);
  switch(f) {
    case 0:
      ssd1306Plugin.display->setFont(ArialMT_Plain_10);
    break;
    case 1:
      ssd1306Plugin.display->setFont(ArialMT_Plain_16);
    break;
    case 2:
      ssd1306Plugin.display->setFont(ArialMT_Plain_24);
    break;
  }
  switch(a) {
    case 0:
      ssd1306Plugin.display->setTextAlignment(TEXT_ALIGN_LEFT);
    break;
    case 1:
      ssd1306Plugin.display->setTextAlignment(TEXT_ALIGN_CENTER);
    break;
    case 2:
      ssd1306Plugin.display->setTextAlignment(TEXT_ALIGN_RIGHT);
    break;
  }
  return 0;
}

// I2C pin 4(SDA) 5(SCL)
// Wire.begin(sda,scl);
int lua_initSSD1306 (lua_State *L) {
  uint8_t sda = luaL_checkinteger(L, 1);
  uint8_t scl = luaL_checkinteger(L, 2);
  ssd1306Plugin.display = new SSD1306Wire(0x3c, sda, scl);
  ssd1306Plugin.display->init();
  ssd1306Plugin.display->setTextAlignment(TEXT_ALIGN_LEFT);
  ssd1306Plugin.display->setFont(ArialMT_Plain_16);
  ssd1306Plugin.display->clear();
  ssd1306Plugin.display->display();
  lua.Lua_register(F("ssd1306drawString"), (const lua_CFunction) &lua_ssd1306drawString);
  lua.Lua_register(F("ssd1306clear"), (const lua_CFunction) &lua_ssd1306clear);
  lua.Lua_register(F("ssd1306display"), (const lua_CFunction) &lua_ssd1306display);
  lua.Lua_register(F("ssd1306font"), (const lua_CFunction) &lua_ssd1306font);
  return 0;
}

plugin_SSD1306::plugin_SSD1306(void) {
  display = NULL;
}

void plugin_SSD1306::setup(void) {
    lua.Lua_register(F("initSSD1306"), (const lua_CFunction) &lua_initSSD1306);
    Serial.println(F("INFO: setup SSD1306 plugin"));
}

void plugin_SSD1306::loop(void) {
}

void plugin_SSD1306::json (AsyncWebServerRequest *request,AsyncResponseStream *response) {

}
