#include "LuaWrapper.h"

/*extern "C" {
  static int lua_wrapper_pinMode(lua_State *lua) {
    int a = luaL_checkinteger(lua, 1);
    int b = luaL_checkinteger(lua, 2);
    pinMode(a, b);
    return 0;
  }

  static int lua_wrapper_digitalWrite(lua_State *lua) {
    int a = luaL_checkinteger(lua, 1);
    int b = luaL_checkinteger(lua, 2);
    digitalWrite(a, b);
    return 0;
  }

  static int lua_wrapper_analogWrite(lua_State *lua) {
    int a = luaL_checkinteger(lua, 1);
    int b = luaL_checkinteger(lua, 2);
    analogWrite(a, b);
    return 0;
  }

  static int lua_wrapper_delay(lua_State *lua) {
    int a = luaL_checkinteger(lua, 1);
    delay(a);
    return 0;
  }

  static int lua_wrapper_print(lua_State *lua) {
    String a = (String)luaL_tolstring(lua, 1, NULL);
    Serial.print(a);
    return 0;
  }

  static int lua_wrapper_tostring(lua_State *lua) {
    const char *temp = luaL_tolstring(lua, 1, NULL);
    lua_pushstring(lua, temp);
    return 1;
  }

  static int lua_wrapper_tonumber(lua_State *lua) {
    float i = lua_tonumber(lua, 1);
    lua_pushnumber(lua, i);
    return 1;
  }

  static int lua_wrapper_millis(lua_State *lua) {
    lua_pushnumber(lua, (lua_Number) millis());
    return 1;
  }
}*/

LuaWrapper::LuaWrapper() {
  _state = luaL_newstate();
  //luaopen_base(_state);
  //luaopen_string(_state);
  //luaopen_math(_state);
  //luaopen_table(_state); //error
  //luaopen_os(_state); //error
  /*Lua_register(F("pinMode"), (const lua_CFunction) &lua_wrapper_pinMode);
  Lua_register(F("digitalWrite"), (const lua_CFunction) &lua_wrapper_digitalWrite);
  Lua_register(F("analogWrite"), (const lua_CFunction) &lua_wrapper_analogWrite);
  Lua_register(F("delay"), (const lua_CFunction) &lua_wrapper_delay);
  Lua_register(F("print"), (const lua_CFunction) &lua_wrapper_print);
  Lua_register(F("millis"), (const lua_CFunction) &lua_wrapper_millis);
  Lua_register(F("tostring"), (const lua_CFunction) &lua_wrapper_tostring);
  Lua_register(F("tonumber"), (const lua_CFunction) &lua_wrapper_tonumber);*/
}

String LuaWrapper::Lua_dostring(const String *script) {
  String result;
  if (luaL_dostring(_state, script->c_str())) {
    result += String(lua_tostring(_state, -1));
    lua_pop(_state, 1);
  }
  return result;
}

String LuaWrapper::Lua_docstring(const char *script) {
  String result;
  if (luaL_dostring(_state, script)) {
    result += String(lua_tostring(_state, -1));
    lua_pop(_state, 1);
  }
  return result;
}

void LuaWrapper::Lua_register(const String name, const lua_CFunction function) {
  lua_register(_state, name.c_str(), function);
}

String LuaWrapper::addConstants() {
  String constants = "INPUT = " + String(INPUT) + "\r\n";
  constants += "OUTPUT = " + String(OUTPUT) + "\r\n";
  constants += "LOW = " + String(LOW) + "\r\n";
  constants += "HIGH = " + String(HIGH) + "\r\n";
  return constants;
}

lua_State *LuaWrapper::getState(void) {
  return _state;
}
