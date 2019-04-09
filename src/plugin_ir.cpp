
#include "plugin_ir.h"

extern plugin_IR irPlugin;
extern LuaWrapper lua;


int lua_getIRValue (lua_State *L) {
  lua_pushinteger(L, irPlugin.getIRValue());  /* push result */
  irPlugin._irrx_result = 0;
  return 1;  /* number of results */
}

int lua_IRsend (lua_State *L) {
  uint16_t type = luaL_checkinteger(L, 1);
  uint64_t data = luaL_checkinteger(L, 2);
  uint16_t nbits = luaL_checkinteger(L, 3);
  irPlugin._irtx->send(type,data,nbits);
  return 0;  /* number of results */
}


int lua_initIR (lua_State *L) {
  uint8_t rx = luaL_checkinteger(L, 1);
  uint8_t tx = luaL_checkinteger(L, 2);
  irPlugin._irrx = new IRrecv(rx);
  irPlugin._irtx = new IRsend(tx);
  Serial.println(F("INFO: setup ir plugin"));
  irPlugin._irrx->enableIRIn();
  irPlugin._irtx->begin();
  lua.Lua_register(F("getIRValue"), (const lua_CFunction) &lua_getIRValue);
  lua.Lua_register(F("IRsend"), (const lua_CFunction) &lua_IRsend);
  return 0;
}

plugin_IR::plugin_IR(void) {

}

void plugin_IR::setup(void) {
  lua.Lua_register(F("initIR"), (const lua_CFunction) &lua_initIR);
  _irrx_result=0;
  Serial.println(F("INFO: setup plugin_IR"));
}

void plugin_IR::loop(void) {
  decode_results ir_results;
  if (_irrx->decode(&ir_results)) {
    #ifdef DEBUGMODE
      Serial.print(F("DEBUG: irrx command="));
      Serial.println((String)ir_results.command);
    #endif
    if(_irrx_result==0) {
      _irrx_result = ir_results.command;
      _irrx->resume();
    }
  }
}

void plugin_IR::json (AsyncWebServerRequest *request,AsyncResponseStream *response) {

}

int plugin_IR::getIRValue(void) {
  return _irrx_result;
}
