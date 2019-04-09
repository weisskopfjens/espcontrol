
#if defined(ESP8266)
#include "plugin_icmpping.h"

extern plugin_ICMPPing icmpping;
extern LuaWrapper lua;

int lua_ping (lua_State *L) {
  const char * ip = luaL_checkstring(L, 1);
  IPAddress pingaddr;
  pingaddr.fromString(ip);
  icmpping.ICMPPing.begin(pingaddr);
  return 0;
}

int lua_pingReceived (lua_State *L) {
  lua_pushinteger(L, icmpping.ICMPPing.response().answer);
  lua_pushstring(L, icmpping.ICMPPing.response().addr.toString().c_str());
  lua_pushinteger(L, icmpping.ICMPPing.response().total_time);
  lua_pushinteger(L, icmpping.ICMPPing.response().total_recv);
  lua_pushinteger(L, icmpping.ICMPPing.response().total_sent);
  return 5;
}

plugin_ICMPPing::plugin_ICMPPing(void) {

}

void plugin_ICMPPing::setup(void) {
  lua.Lua_register(F("ping"), (const lua_CFunction) &lua_ping);
  lua.Lua_register(F("pingReceived"), (const lua_CFunction) &lua_pingReceived);
  Serial.println(F("INFO: setup plugin_ICMPPing"));
}

void plugin_ICMPPing::loop(void) {

}

void plugin_ICMPPing::json(AsyncWebServerRequest *request,AsyncResponseStream *response) {

}
#endif
