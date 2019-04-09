
#include "plugin_rc.h"

extern plugin_RC rcPlugin;
extern LuaWrapper lua;

int lua_getRCValue (lua_State *L) {
  lua_pushinteger(L, rcPlugin.getValue());  /* push result */
  rcPlugin._rcvalue = 0;
  return 1;  /* number of results */
}

int lua_RCswitchOn (lua_State *L) {
  const char * dip1 = luaL_checkstring(L, 1);
  const char * dip2 = luaL_checkstring(L, 2);
  rcPlugin.rcSwitch.switchOn(dip1, dip2);
  return 0;  /* number of results */
}

int lua_RCswitchOff (lua_State *L) {
  const char * dip1 = luaL_checkstring(L, 1);
  const char * dip2 = luaL_checkstring(L, 2);
  rcPlugin.rcSwitch.switchOff(dip1, dip2);
  return 0;  /* number of results */
}

int lua_RCsend (lua_State *L) {
  int data1 = luaL_checkinteger(L, 1);
  int data2 = luaL_checkinteger(L, 2);
  rcPlugin.rcSwitch.send(data1, data2);
  return 0;  /* number of results */
}

int lua_initRCtx (lua_State *L) {
  uint8_t pin = luaL_checkinteger(L, 1);
  // Transmitter is connected to Arduino Pin
  rcPlugin.rcSwitch.enableTransmit(pin);
  lua.Lua_register(F("RCswitchOn"), (const lua_CFunction) &lua_RCswitchOn);
  lua.Lua_register(F("RCswitchOff"), (const lua_CFunction) &lua_RCswitchOff);
  lua.Lua_register(F("RCsend"), (const lua_CFunction) &lua_RCsend);
  return 0;
}

int lua_initRCrx (lua_State *L) {
  uint8_t pin = luaL_checkinteger(L, 1);
  // Receiver is connected to Arduino Pin
  rcPlugin.rcSwitch.enableReceive(pin);
  lua.Lua_register(F("getRCValue"), (const lua_CFunction) &lua_getRCValue);
  return 0;
}

plugin_RC::plugin_RC(void) {

}

void plugin_RC::setup(void) {
  lua.Lua_register(F("initRCtx"), (const lua_CFunction) &lua_initRCtx);
  lua.Lua_register(F("initRCrx"), (const lua_CFunction) &lua_initRCrx);
  Serial.println(F("INFO: setup plugin_RC"));
}

void plugin_RC::loop(void) {
  if (rcSwitch.available()) {
    _rcvalue = rcSwitch.getReceivedValue();
  }
}

void plugin_RC::json (AsyncWebServerRequest *request,AsyncResponseStream *response) {

}

int plugin_RC::getValue(void) {
  return _rcvalue;
}
