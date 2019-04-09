
#include "plugin_mqtt.h"

extern plugin_MQTT mqttPlugin;
extern LuaWrapper lua;

int lua_mqttSubscribe (lua_State *L) {
  const char *topic = luaL_checkstring(L, 1);
  int qos = luaL_checkinteger(L, 2);
  uint16_t packetIdSub = mqttPlugin.mqttClient.subscribe(topic, qos);
  lua_pushinteger(L, packetIdSub);
  return 1;
}

int lua_mqttPublish (lua_State *L) {
  const char *topic = luaL_checkstring(L, 1);
  int qos = luaL_checkinteger(L, 2);
  bool retain = luaL_checkinteger(L, 3);
  const char *message = luaL_checkstring(L, 4);
  uint16_t packetIdPub = mqttPlugin.mqttClient.publish(topic, qos, retain, message);
  lua_pushinteger(L, packetIdPub);
  return 1;
}

int lua_mqttSetWill (lua_State *L) {
  const char *topic = luaL_checkstring(L, 1);
  int qos = luaL_checkinteger(L, 2);
  bool retain = luaL_checkinteger(L, 3);
  const char *message = luaL_checkstring(L, 4);
  uint16_t packetIdPub = mqttPlugin.mqttClient.publish(topic, qos, retain, message);
  lua_pushinteger(L, packetIdPub);
  return 1;
}

int lua_initMQTT (lua_State *L) {
  IPAddress ip;
  ip.fromString(luaL_checkstring(L, 1));
  uint16_t port = luaL_checkinteger(L, 2);
  mqttPlugin.mqttClient.setServer(ip, port);
  lua.Lua_register(F("mqttSubscribe"), (const lua_CFunction) &lua_mqttSubscribe);
  lua.Lua_register(F("mqttPublish"), (const lua_CFunction) &lua_mqttPublish);
  lua.Lua_register(F("mqttSetWill"), (const lua_CFunction) &lua_mqttSetWill);
  return 0;
}


void connectToMqtt() {
  Serial.println("INFO: Connecting to MQTT...");
  mqttPlugin.mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println(F("INFO: Connected to MQTT."));
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println(F("INFO: Disconnected from MQTT."));
  if (WiFi.isConnected()) {
    mqttPlugin.mqttReconnectTimer.once(2, connectToMqtt);
  } else {
    mqttPlugin.mqttReconnectTimer.detach();
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {

}

void onMqttUnsubscribe(uint16_t packetId) {

}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  #ifdef DEBUGMODE
    Serial.println("Publish received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  qos: ");
    Serial.println(properties.qos);
    Serial.print("  dup: ");
    Serial.println(properties.dup);
    Serial.print("  retain: ");
    Serial.println(properties.retain);
    Serial.print("  len: ");
    Serial.println(len);
    Serial.print("  index: ");
    Serial.println(index);
    Serial.print("  total: ");
    Serial.println(total);
  #endif
}

void onMqttPublish(uint16_t packetId) {
  #ifdef DEBUGMODE
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
  #endif
}


plugin_MQTT::plugin_MQTT( void ) {

}

void plugin_MQTT::setup(void) {
  lua.Lua_register(F("initMQTT"), (const lua_CFunction) &lua_initMQTT);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  Serial.println(F("INFO: setup plugin_MQTT"));
}

void plugin_MQTT::loop(void) {

}

void plugin_MQTT::json (AsyncWebServerRequest *request,AsyncResponseStream *response) {

}
