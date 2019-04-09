#include "core.h"
#include "staticweb.h"

ESPNode *knownESPNodes[MAXNODES];
rst_info *ESPresetInfo;
LuaWrapper lua;
String luaresult;

// #############################################################
//  ENABLE / DISABLE PLUGIN
// #############################################################
//plugin_WS2812 ws2812pi;
//plugin_ICMPPing icmpping;
//plugin_RC rcPlugin;
//plugin_DHT dhtPlugin;
//plugin_SSD1306 ssd1306Plugin;
plugin_BME280 bme280Plugin;
//plugin_MQTT mqttPlugin;
plugin_IR irPlugin;
plugin_LEDstrip ledsPlugin;
Core core;


static int lua_func_pinMode(lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  int b = luaL_checkinteger(L, 2);
  pinMode(a, b);
  return 0;
}

int lua_func_digitalWrite(lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  int b = luaL_checkinteger(L, 2);
  digitalWrite(a, b);
  return 0;
}

int lua_func_digitalRead(lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  lua_pushnumber(L, digitalRead(a));
  return 1;
}

int lua_func_analogWrite(lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  int b = luaL_checkinteger(L, 2);
  analogWrite(a, b);
  return 0;
}

int lua_func_delay(lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  delay(a);
  return 0;
}

int lua_func_print(lua_State *L) {
  String a = (String)luaL_tolstring(L, 1, NULL);
  Serial.print(a);
  return 0;
}

int lua_func_tostring(lua_State *L) {
  const char *temp = luaL_tolstring(L, 1, NULL);
  lua_pushstring(L, temp);
  return 1;
}

int lua_func_tonumber(lua_State *L) {
  float i = lua_tonumber(L, 1);
  lua_pushnumber(L, i);
  return 1;
}

int lua_func_millis(lua_State *L) {
  lua_pushnumber(L, (lua_Number) millis());
  return 1;
}

int lua_func_deleteFile(lua_State *L) {
  const char *filename=luaL_checkstring(L, 1);
  SPIFFS.remove(filename);
  return 0;
}

int lua_func_renameFile(lua_State *L) {
  const char *fromfilename=luaL_checkstring(L, 1);
  const char *tofilename=luaL_checkstring(L, 2);
  SPIFFS.rename(fromfilename, tofilename);
  return 0;
}

int lua_func_format(lua_State *L) {
  SPIFFS.format();
  return 0;
}

int lua_func_getCounter(lua_State *L) {
  int cnr=luaL_checkinteger(L, 1);
  lua_pushinteger(L, core.counter[cnr]);
  return 1;
}

int lua_func_resetCounter(lua_State *L) {
  int cnr=luaL_checkinteger(L, 1);
  core.counter[cnr]=0;
  return 0;
}

int lua_func_attachCounter(lua_State *L) {
  int cnr=luaL_checkinteger(L, 1);
  int cpin=luaL_checkinteger(L, 2);
  int ctype=luaL_checkinteger(L, 3);
  // #define RISING    0x01
  // #define FALLING   0x02
  // #define CHANGE    0x03
  int cpullup=luaL_checkinteger(L, 4);
  // 0 = INPUT
  // 2 = INPUT_PULLUP
  core.counter[cnr]=0;
  pinMode(cpin, cpullup);
  switch(cnr) {
    case 0:
      attachInterrupt(digitalPinToInterrupt(cpin), handleInterrupt1, ctype);
    break;
    case 1:
      attachInterrupt(digitalPinToInterrupt(cpin), handleInterrupt2, ctype);
    break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(cpin), handleInterrupt3, ctype);
    break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(cpin), handleInterrupt4, ctype);
    break;
  }
  return 0;
}

int lua_func_httpGet(lua_State *L) {
  const char *data = luaL_checkstring(L, 1);
  HTTPClient http;
  #ifdef DEBUGMODE
    Serial.print(F("DEBUG: httpget: "));
    Serial.println(data);
  #endif
  http.begin(data);
  http.GET();
  http.end();
  return 0;
}

int lua_func_udpBroadcast(lua_State *L) {
  const char *message = luaL_checkstring(L, 1);  /* get argument */
  uint udpport = luaL_checkinteger(L, 2);  /* get argument */
  core.udpClient.broadcastTo(message, udpport);
  return 0;
}

int lua_func_udpSend(lua_State *L) {
  const char *ip = luaL_checkstring(L, 1);
  const char *message = luaL_checkstring(L, 2);  /* get argument */
  uint udpport = luaL_checkinteger(L, 3);  /* get argument */
  IPAddress ipaddress;
  ipaddress.fromString(ip);
  core.udpClient.writeTo((uint8_t*)message, strlen(message), ipaddress, udpport);
  return 0;
}

int lua_func_sendMail(lua_State *L) {
  const char *mailhost = luaL_checkstring(L, 1);  /* get argument */
  uint mailport = luaL_checkinteger(L, 2);  /* get argument */
  const char *mailuser = luaL_checkstring(L, 3);  /* get argument */
  const char *mailpass = luaL_checkstring(L, 4);  /* get argument */
  const char *from = luaL_checkstring(L, 5);  /* get argument */
  const char *to = luaL_checkstring(L, 6);  /* get argument */
  const char *subject = luaL_checkstring(L, 7);  /* get argument */
  const char *message = luaL_checkstring(L, 8);  /* get argument */
  SendEmail email(mailhost,mailport,mailuser,mailpass, 5000,0);
  if(email.send(from, to, subject, message)) {
    lua_pushboolean(L, true);
  } else {
    lua_pushboolean(L, false);
  }
  return 1;
}

int lua_func_minOfDay(lua_State *L) {
  time_t t = now();
  lua_pushinteger(L, hour(t)*60+minute(t));
  return 1;
}

int lua_func_wifiDisconnect (lua_State *L) {
  WiFi.disconnect(true);
  return 0;  /* number of results */
}

int lua_func_reboot (lua_State *L) {
  Serial.println(F("INFO: Rebooting..."));
  delay(2000);
  ESP.restart();
  return 0;  /* number of results */
}

int lua_func_saveCfg (lua_State *L) {
  Serial.println(F("INFO: Save config"));
  core.cfg.write();
  return 0;  /* number of results */
}

int lua_func_readCfg (lua_State *L) {
  Serial.println(F("INFO: Read config"));
  core.cfg.read();
  return 0;  /* number of results */
}

int lua_func_now (lua_State *L) {
  lua_pushinteger(L, now());
  return 1;  /* number of results */
}

int lua_func_ntpBegin (lua_State *L) {
  const char *server = luaL_checkstring(L, 1);
  int tz = luaL_checkinteger(L, 2);
  int daylight = luaL_checkinteger(L, 3);
  NTP.begin(server,tz,daylight);
  return 0;
}

int lua_func_time (lua_State *L) {
  lua_pushstring(L, NTP.getTimeDateString().c_str());
  return 1;  /* number of results */
}

int lua_func_sunrise (lua_State *L) {
  float lat = luaL_checknumber(L, 1);
  float lon = luaL_checknumber(L, 2);
  float tz = luaL_checknumber(L, 3);
  time_t t = now();
  Dusk2Dawn location = Dusk2Dawn(lat, lon, tz);
  int min = location.sunrise( year(t), month(t), day(t), NTP.isSummerTime() );
  lua_pushinteger(L, min);
  return 1;  /* number of results */
}

int lua_func_sunset (lua_State *L) {
  float lat = luaL_checknumber(L, 1);
  float lon = luaL_checknumber(L, 2);
  float tz = luaL_checknumber(L, 3);
  time_t t = now();
  Dusk2Dawn location = Dusk2Dawn(lat, lon, tz);
  int min = location.sunset( year(t), month(t), day(t), NTP.isSummerTime() );
  lua_pushinteger(L, min);
  return 1;  /* number of results */
}

int lua_func_newNode (lua_State *L) {
  size_t s;
  const char *name=NULL;
  const char *ip=NULL;
  unsigned char id;
  name = luaL_checklstring(L, 1, &s);  /* get argument */
  id = luaL_checkinteger(L, 2);  /* get argument */
  if(core.knownESPNodes[id]==NULL) {
    core.knownESPNodes[id] = (ESPNode*)calloc(1,sizeof(ESPNode));
  } else {
    free(core.knownESPNodes[id]->name);
  }
  core.knownESPNodes[id]->name = (char*)calloc(s+1,sizeof(char));
  strcpy(core.knownESPNodes[id]->name,name);
  ip = luaL_checklstring(L, 3, &s);  /* get argument */
  core.knownESPNodes[id]->ip.fromString(ip);
  core.knownESPNodes[id]->lastsignal = millis();
  return 0;  /* number of results */
}

int lua_func_getNode (lua_State *L) {
  uint id;
  id = luaL_checkinteger(L, 1);  /* get argument */
  if( core.knownESPNodes[id]==NULL or id>=MAXNODES ) {
    return 0;
  }
  lua_pushstring(L, core.knownESPNodes[id]->name);
  lua_pushstring(L, core.knownESPNodes[id]->ip.toString().c_str() );
  lua_pushinteger(L, core.knownESPNodes[id]->lastsignal);
  return 3;  /* number of results */
}

int lua_func_configWiFi (lua_State *L) {
  const char *ssid = luaL_checkstring(L, 1);
  const char *wlankey = luaL_checkstring(L, 2);
  WiFi.disconnect();
  core.cfg.get("ssid")->setValue(ssid);
  core.cfg.get("wlankey")->setValue(wlankey);
  yield();
  WiFi.begin(core.cfg.get("ssid")->getValue(),core.cfg.get("wlankey")->getValue());
  return 0;
}

int lua_func_round (lua_State *L) {
  float value = luaL_checknumber(L, 1);
  lua_pushnumber(L, roundf(value));
  return 1;
}

int lua_func_floor (lua_State *L) {
  float value = luaL_checknumber(L, 1);
  lua_pushnumber(L, floor(value));
  return 1;
}

int lua_func_ceil (lua_State *L) {
  float value = luaL_checknumber(L, 1);
  lua_pushnumber(L, ceil(value));
  return 1;
}

int lua_func_abs (lua_State *L) {
  int value = luaL_checkinteger(L, 1);
  lua_pushinteger(L, abs(value));
  return 1;
}

int lua_func_strcmp (lua_State *L) {
  const char * str1 = luaL_checkstring(L, 1);
  const char * str2 = luaL_checkstring(L, 2);
  lua_pushinteger(L, strcmp(str1, str2));
  return 1;
}

int lua_func_strstr (lua_State *L) {
  const char * str1 = luaL_checkstring(L, 1);
  const char * str2 = luaL_checkstring(L, 2);
  if(strstr(str1, str2)) {
    lua_pushboolean(L, true);
  } else {
    lua_pushboolean(L, false);
  }
  return 1;
}

int lua_func_deepSleep (lua_State *L) {
  long value = luaL_checkinteger(L, 1);
  ESP.deepSleep(value);
  return 0;
}

Core::Core(void) {
  //httpRequest=NULL;
}

/***************************************
 Core setup function
***************************************/
void Core::setup(void) {
  lua.Lua_register(F("pinMode"), (const lua_CFunction) &lua_func_pinMode);
  lua.Lua_register(F("digitalWrite"), (const lua_CFunction) &lua_func_digitalWrite);
  lua.Lua_register(F("digitalRead"), (const lua_CFunction) &lua_func_digitalRead);
  lua.Lua_register(F("analogWrite"), (const lua_CFunction) &lua_func_analogWrite);
  lua.Lua_register(F("delay"), (const lua_CFunction) &lua_func_delay);
  lua.Lua_register(F("print"), (const lua_CFunction) &lua_func_print);
  lua.Lua_register(F("millis"), (const lua_CFunction) &lua_func_millis);
  lua.Lua_register(F("tostring"), (const lua_CFunction) &lua_func_tostring);
  lua.Lua_register(F("tonumber"), (const lua_CFunction) &lua_func_tonumber);
  lua.Lua_register(F("reboot"), (const lua_CFunction) &lua_func_reboot);
  lua.Lua_register(F("saveCfg"), (const lua_CFunction) &lua_func_saveCfg);
  lua.Lua_register(F("readCfg"), (const lua_CFunction) &lua_func_readCfg);
  lua.Lua_register(F("now"), (const lua_CFunction) &lua_func_now);
  lua.Lua_register(F("ntpBegin"), (const lua_CFunction) &lua_func_ntpBegin);
  lua.Lua_register(F("time"), (const lua_CFunction) &lua_func_time);
  lua.Lua_register(F("sunrise"), (const lua_CFunction) &lua_func_sunrise);
  lua.Lua_register(F("sunset"), (const lua_CFunction) &lua_func_sunset);
  lua.Lua_register(F("newNode"), (const lua_CFunction) &lua_func_newNode);
  lua.Lua_register(F("wifiDisconnect"), (const lua_CFunction) &lua_func_wifiDisconnect);
  lua.Lua_register(F("minOfDay"), (const lua_CFunction) &lua_func_minOfDay);
  lua.Lua_register(F("sendMail"), (const lua_CFunction) &lua_func_sendMail);
  lua.Lua_register(F("udpBroadcast"), (const lua_CFunction) &lua_func_udpBroadcast);
  lua.Lua_register(F("udpSend"), (const lua_CFunction) &lua_func_udpSend);
  lua.Lua_register(F("httpGet"), (const lua_CFunction) &lua_func_httpGet);
  lua.Lua_register(F("getNode"), (const lua_CFunction) &lua_func_getNode);
  lua.Lua_register(F("configWiFi"), (const lua_CFunction) &lua_func_configWiFi);
  lua.Lua_register(F("attachCounter"), (const lua_CFunction) &lua_func_attachCounter);
  lua.Lua_register(F("getCounter"), (const lua_CFunction) &lua_func_getCounter);
  lua.Lua_register(F("resetCounter"), (const lua_CFunction) &lua_func_resetCounter);
  lua.Lua_register(F("deleteFile"), (const lua_CFunction) &lua_func_deleteFile);
  lua.Lua_register(F("format"), (const lua_CFunction) &lua_func_format);
  lua.Lua_register(F("renameFile"), (const lua_CFunction) &lua_func_renameFile);
  lua.Lua_register(F("round"), (const lua_CFunction) &lua_func_round);
  lua.Lua_register(F("floor"), (const lua_CFunction) &lua_func_floor);
  lua.Lua_register(F("ceil"), (const lua_CFunction) &lua_func_ceil);
  lua.Lua_register(F("abs"), (const lua_CFunction) &lua_func_abs);
  lua.Lua_register(F("strcmp"), (const lua_CFunction) &lua_func_strcmp);
  lua.Lua_register(F("strstr"), (const lua_CFunction) &lua_func_strstr);
  lua.Lua_register(F("deepSleep"), (const lua_CFunction) &lua_func_deepSleep);

  pinMode(ONBOARDLED,OUTPUT);
  digitalWrite(ONBOARDLED,LOW);
  _starttime = now();
  _rulesActive = true;
  serialcommand.reserve(80);
  command.reserve(80);
  _scriptexecute = false;
  Serial.println(F("Starting up core"));
  Serial.print(F("Last restart cause: "));
  Serial.println((String)ESP.getResetReason());
  Serial.print(F("ChipID: "));
  Serial.println((String)ESP.getChipId());
  Serial.print(F("CoreVersion: "));
  Serial.println((String)ESP.getCoreVersion());
  Serial.print(F("SDKVersion: "));
  Serial.println((String)ESP.getSdkVersion());
  Serial.print(F("SketchSize: "));
  Serial.println((String)ESP.getSketchSize());
  Serial.print(F("FreeSketchSpace: "));
  Serial.println((String)ESP.getFreeSketchSpace());
  Serial.print(F("SketchMD5: "));
  Serial.println((String)ESP.getSketchMD5());
  Serial.print(F("INFO: Mounting FS..."));
  if (!SPIFFS.begin()) {
    Serial.println(F("FAIL"));
    Serial.println(F("ERROR: Failed to mount file system"));
    return;
  } else {
    Serial.println(F("DONE"));
  }
  cfg.addKey("ssid","",32);
  cfg.addKey("wlankey","",32);
  cfg.addKey("ip","",16);
  cfg.addKey("subnet","",16);
  cfg.addKey("gateway","",16);
  cfg.addKey("dns1","",16);
  cfg.addKey("dns2","",16);
  cfg.addKey("id","10",2);
  cfg.addKey("name","undefined",32);
  cfg.addKey("user","admin",32);
  cfg.addKey("password","admin",32);
  cfg.addKey("udpport","28000",6);
  cfg.addKey("rulesact","1",1);
  cfg.addKey("rules",2048);
  switch(cfg.read()) {
    case E_CONFIG_OK:
      Serial.println(F("INFO: Config read"));
      break;
    case E_CONFIG_FS_ACCESS:
      Serial.println(F("ERROR: Couldn't access file system"));
      break;
    case E_CONFIG_FILE_NOT_FOUND:
      Serial.println(F("ERROR: File not found"));
      cfg.write();
      break;
    case E_CONFIG_FILE_OPEN:
      Serial.println(F("ERROR: Couldn't open file"));
      break;
    case E_CONFIG_PARSE_ERROR:
      Serial.println(F("ERROR: File was not parsable"));
      break ;
  }

  // initial knownESPNodes memory
  for(int i=0;i<MAXNODES;i++) {
    knownESPNodes[i]=NULL;
  }

  // register WiFi callbacks
  stationGotIPHandler = WiFi.onStationModeGotIP (onSTAGotIP);// As soon WiFi is connected, start NTP Client
  stationDisconnectedHandler = WiFi.onStationModeDisconnected (onSTADisconnected);
  stationConnectedHandler = WiFi.onStationModeConnected (onSTAConnected);
  apStationConnectedHandler = WiFi.onSoftAPModeStationConnected (onAP_STAConnected);
  apStationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected (onAP_STADisconnected);

  Serial.println (F("INFO: Starting WLAN"));
  LEDTicker.attach(1, statusLED);
  WiFi.mode(WIFI_STA);
  Serial.println (F("WLAN: WiFi.mode(WIFI_STA)"));
  WiFi.begin(cfg.get("ssid")->getValue(),cfg.get("wlankey")->getValue());
  // setup static ip configuration
  if( (String)cfg.get("ip")->getValue()!="" && (String)cfg.get("subnet")->getValue()!="" && (String)cfg.get("gateway")->getValue()!="" ) {
    //set static ip
    IPAddress _ip,_gw,_sn,_dns1,_dns2;
    _ip.fromString(cfg.get("ip")->getValue());
    _gw.fromString(cfg.get("gateway")->getValue());
    _sn.fromString(cfg.get("subnet")->getValue());
    _dns1.fromString(cfg.get("dns1")->getValue());
    _dns2.fromString(cfg.get("dns2")->getValue());
    if(WiFi.config(_ip,_gw,_sn,_dns1,_dns2)) {
      Serial.print(F("INFO: Using static IP config."));
      Serial.println(WiFi.localIP());
      Serial.println(cfg.get("ip")->getValue());
    } else {
      Serial.println(F("ERROR: Can't use static IP config."));
    }
  }
  Serial.print (F("INFO: Hostname = "));
  Serial.println (cfg.get("name")->getValue());
  WiFi.hostname(cfg.get("name")->getValue());
  //WiFi.enableSTA(true);
  //WiFi.enableAP(false);

  // ntp callback
  NTP.onNTPSyncEvent ([](NTPSyncEvent_t ntpEvent) {
    if (ntpEvent < 0) {
      Serial.printf_P (PSTR("ERROR: Time Sync error: %d\n"), ntpEvent);
      if (ntpEvent == noResponse)
        Serial.println (F("ERROR: NTP server not reachable"));
      else if (ntpEvent == invalidAddress)
        Serial.println (F("ERROR: Invalid NTP server address"));
      else if (ntpEvent == errorSending)
        Serial.println (F("ERROR: Error sending request"));
      else if (ntpEvent == responseError)
        Serial.println (F("ERROR: NTP response error"));
    } else {
      if (ntpEvent == timeSyncd) {
        Serial.print (F("INFO: Got NTP time: "));
        Serial.print (NTP.getTimeDateString ());
        Serial.print (" ");
        Serial.print (NTP.isSummerTime () ? F("Summer Time. ") : F("Winter Time. "));
        Serial.print (F("Uptime: "));
        Serial.print (NTP.getUptimeString ());
        Serial.print (F(" since "));
        Serial.println (NTP.getTimeDateString (NTP.getFirstSync ()).c_str ());
      }
    }
  });

  webServer.on("/update", HTTP_GET, handleUpdateform);
  webServer.on("/update", HTTP_POST,handleUpdate,handleUpdateUpload);
  webServer.on("/control", HTTP_GET,handleControl );
  webServer.on("/config", HTTP_ANY,handleConfig );
  webServer.on("/json", HTTP_ANY,handleJSON );
  webServer.on("/iro.min.js", HTTP_ANY, handle_FILE_iro);
  webServer.on("/w3.css", HTTP_ANY, handle_FILE_w3css);
  webServer.on("/filemanager", HTTP_ANY, handle_FILE_filemanager);
  webServer.on("/upload", HTTP_GET,handleFiles );
  webServer.on("/upload", HTTP_POST,handleUpload,handleFileUpload);
  webServer.serveStatic("/", SPIFFS, "/");
  webServer.begin();
  Serial.println(F("INFO: Webserver started."));

  // rules enabled in config?
  _rulesActive = atoi(cfg.get("rulesact")->getValue());
  // check if rule causes an exception and a resulting reboot
  // if yes, then disable the rule
  // 0 = Power reboot
  // 1 = Hardware WDT *
  // 2 = Fatal exception *
  // 3 = Software watchdog *
  // 4 = Software reset
  // 5 = Deep sleep
  // 6 = Hardware reset
  if(ESPresetInfo->reason>0 && ESPresetInfo->reason<=3) { //Hardware Watchdog
    Serial.print (F("ERROR: Exeption or watchdog! Rules disabled"));
    _rulesActive = 0;
  }
}

/***************************************
 Core loop function
***************************************/
void Core::loop(void) {
  unsigned long temp1 = micros() - _lastIteration;
  avrIteration = (temp1 + avrIteration) / 2;
  _lastIteration = micros();
  /***************************************
   Run rules
  ***************************************/
  if( millis() >= _lastRulesRun+RULEINTERVAL && _rulesActive) {
    _lastRulesRun = millis();
    if (luaL_dostring(lua.getState(), cfg.get("rules")->getValue())) {
      luaresult = String(lua_tostring(lua.getState(), -1));
      lua_pop(lua.getState(), 1);
      Serial.println(F(""));
      Serial.println(luaresult);
      Serial.println(F("ERROR: LUA syntax error. Rules disabled."));
      _rulesActive = 0;
    }
    lua_gc(lua.getState(),LUA_GCCOLLECT,0);
  }
  /***************************************
   UDP network send node data
  ***************************************/
  if(millis()>_lastudpsend+UDPNEWNODEINTERVAL) {
    updateNodes(); // checks for timed out nodes and delete them
    _lastudpsend=millis();
    char message[64];
    strcpy(message, "newNode(\"");
    strcat(message, core.cfg.get("name")->getValue());
    strcat(message, "\",");
    strcat(message, core.cfg.get("id")->getValue());
    strcat(message, ",\"");
    strcat(message, WiFi.localIP().toString().c_str());
    strcat(message, "\")");
    udpClient.broadcastTo(message, atoi(core.cfg.get("udpport")->getValue()));
  }
  /***************************************
   Serial input handling
  ***************************************/
  if(Serial.available() > 0) {
    char c;
    c = Serial.read();
    Serial.write(c);
    if(c == '\r') {
      Serial.write('\n');
      _scriptexecute = true;
    } else if(c == '\b') {
      serialcommand.remove(serialcommand.length()-1);
    } else if(c != '\n') {
      serialcommand += c;
    }
  }
  if(serialcommand.length() > 0 && _scriptexecute) {
    Serial.println(lua.Lua_dostring(&serialcommand));
    lua_gc(lua.getState(),LUA_GCCOLLECT,0);
    Serial.println((String)ESP.getFreeHeap());
    serialcommand = "";
    _scriptexecute = false;
  }
  /***************************************
   execute commands
  ***************************************/
  if(command.length()>0) {
    lua.Lua_dostring(&command);
    lua_gc(lua.getState(),LUA_GCCOLLECT,0);
    command = "";
  }
}

void Core::json(AsyncWebServerRequest *request,AsyncResponseStream *response) {
  if (request->hasParam("nodes")) {
    bool init=false;
    response->print(F("{\"nodes\":["));
    for(uint id=0;id<MAXNODES;id++) {
      if(knownESPNodes[id]!=NULL) {
        if(init==false) {
          init=true;
        } else {
          response->print(",");
        }
        response->print(F("{"));
        response->print(F("\"ip\":\""));
        response->print( (String)knownESPNodes[id]->ip[0] + "." + (String)knownESPNodes[id]->ip[1] + "." + (String)knownESPNodes[id]->ip[2] + "." + (String)knownESPNodes[id]->ip[3] +"\",");
        response->print(F("\"name\":\""));
        response->print( (String)knownESPNodes[id]->name + "\",");
        response->print(F("\"lastsignal\":"));
        response->print( (String)knownESPNodes[id]->lastsignal);
        response->print(F("}"));
      }
    }
    //response->print(F("]}"));
    response->print(F("]}"));
  } else if (request->hasParam("infos")) {
    response->print(F("{\"infos\":"));
    response->print(F("{\"time\":\""));
    response->print(NTP.getTimeDateString()+"\",");
    response->print(F("\"freeheap\":\""));
    response->print((String)ESP.getFreeHeap()+"\",");
    response->print(F("\"resetreason\":\""));
    response->print((String)ESP.getResetReason()+"\",");
    response->print(F("\"chipid\":\""));
    response->print((String)ESP.getChipId()+"\",");
    response->print(F("\"coreversion\":\""));
    response->print((String)ESP.getCoreVersion()+"\",");
    response->print(F("\"sdkversion\":\""));
    response->print((String)ESP.getSdkVersion()+"\",");
    response->print(F("\"sketchsize\":\""));
    response->print((String)ESP.getSketchSize()+"\",");
    response->print(F("\"freesketchsize\":\""));
    response->print((String)ESP.getFreeSketchSpace()+"\",");
    response->print(F("\"sketchmd5\":\""));
    response->print((String)ESP.getSketchMD5()+"\",");
    response->print(F("\"uptime\":\""));
    response->print((String)(millis()/1000)+"\",");
    response->print(F("\"nodename\":\""));
    response->print((String)cfg.get("name")->getValue()+"\",");
    response->print(F("\"avrIteration\":\""));
    response->print((String)core.avrIteration+"\"");
    response->print(F("}}"));
  } else if ( request->hasParam("files") ) {
    bool init=false;
    Dir dir = SPIFFS.openDir(request->getParam("files")->value().c_str());
    //response->print(F("{\"files\":["));
    response->print(F("{\"files\":["));
    while (dir.next()) {
        if(init==false) {
          init=true;
        } else {
          response->print(",");
        }
        response->print(F("{"));
        response->print(F("\"filename\":\""));
        response->print(dir.fileName());
        response->print(F("\","));
        File f = dir.openFile("r");
        response->print(F("\"size\":"));
        response->print(f.size());
        response->print(F("}"));
    }
    //response->print(F("]}"));
    response->print(F("]}"));
  } else if ( request->hasParam("vars") ) {
    //AsyncResponseStream *response = request->beginResponseStream(F("text/html"));
    response->print(F("{\"vars\":{"));
    bool init=false;
    lua_pushglobaltable(lua.getState());       // Get global table
    lua_pushnil(lua.getState());               // put a nil key on stack
    while (lua_next(lua.getState(),-2) != 0) { // key(-1) is replaced by the next key(-1) in table(-2)
      int key_type = lua_type(lua.getState(), -2);
      int value_type = lua_type(lua.getState(), -1);
      if (key_type != LUA_TSTRING) {
        lua_pop(lua.getState(), 1); // pop the value so that the top contains the key for the next iteration
        continue;
      }
      // support only number, boolean and string values
      if (value_type != LUA_TNUMBER &&
        value_type != LUA_TBOOLEAN &&
        value_type != LUA_TSTRING ) {
        lua_pop(lua.getState(), 1); // again, pop the value before going to the next loop iteration
        continue;
      }
      // get the key as a string
      const char * key_string = lua_tostring(lua.getState(), -2); // no copy required - we already know this is a string
      const char * value_string = lua_tostring(lua.getState(), -1);
      if(init==true) {
        response->print(F(","));
      }
      response->print(F("\""));
      response->print(key_string);
      response->print(F("\":\""));
      response->print(value_string);
      response->print(F("\""));
      init=true;
      lua_pop(lua.getState(),1);
    }
    lua_pop(lua.getState(),1);
    response->print(F("}}"));
  }
}

void Core::updateNodes(void) {
  for(uint id=0;id<MAXNODES;id++) {
    yield();
    if(knownESPNodes[id]!=NULL) {
      if(millis()>=(knownESPNodes[id]->lastsignal+UDPNODETIMEOUT)) {
        deleteNode(id);
        Serial.print(F("INFO: Node id:"));
        Serial.print((String)id);
        Serial.println(F(" timed out."));
      }
    }
  }
}

void Core::deleteNode(uint8 id) {
  if(core.knownESPNodes[id]==NULL) {
    return;
  } else {
    free(core.knownESPNodes[id]->name);
    free(core.knownESPNodes[id]);
    core.knownESPNodes[id] = NULL;
  }
}

/***************************************
 Main setup function
***************************************/

void setup() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(true);
  Serial.begin(115200);
  #ifdef DEBUGMODE
  Serial.setDebugOutput(true);
  Serial.println(F("D E B U G M O D E"));
  #endif
  Serial.println(F(";-)"));
  // check if rule causes an exception and a resulting reboot
  // if yes, then disable plugins
  // 0 = Power reboot
  // 1 = Hardware WDT *
  // 2 = Fatal exception *
  // 3 = Software watchdog *
  // 4 = Software reset
  // 5 = Deep sleep
  // 6 = Hardware reset
  ESPresetInfo = ESP.getResetInfoPtr();
  if(ESPresetInfo->reason>0 && ESPresetInfo->reason<=3) { //Hardware Watchdog
    Serial.println(F("ERROR: All plugins disabled!"));
    core.setup();
  } else {
    Plugin::setupAll();
  }
}

/***************************************
 Main loop function
***************************************/

void loop() {
  if(ESPresetInfo->reason>0 && ESPresetInfo->reason<=3) { //Hardware Watchdog
    core.loop();
  } else {
    Plugin::loopAll();
  }
}

void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
  core.LEDTicker.detach();
  digitalWrite(ONBOARDLED,HIGH);
  Serial.printf_P (PSTR("INFO: Got IP: %s\r\n"), ipInfo.ip.toString ().c_str ());
  Serial.printf_P (PSTR("INFO: Connected: %s\r\n"), WiFi.status () == WL_CONNECTED ? "yes" : "no");
  Serial.println (F("INFO: taskUDPDiscover enabled"));
  if(core.udpClient.listen(atoi(core.cfg.get("udpport")->getValue()))) {
    Serial.print(F("INFO: UDP Listening on IP: "));
    Serial.println(WiFi.localIP());
    core.udpClient.onPacket(handleUDPPacket);
  }
}

// On station connected
void onSTAConnected (WiFiEventStationModeConnected ipInfo) {
  Serial.printf_P (PSTR("INFO: Connected to %s\r\n"), ipInfo.ssid.c_str ());
  core.connectionAttempts=0;
}

// On Soft AP station connected
void onAP_STAConnected (WiFiEventSoftAPModeStationConnected ipInfo) {
  core.LEDTicker.attach(0.2, statusLED);
  Serial.println(F("INFO: Station connected to softAP."));
  Serial.println(F("INFO: Entered config mode"));
  Serial.println(WiFi.softAPIP());
}

// On Soft AP station disconnected
void onAP_STADisconnected (WiFiEventSoftAPModeStationDisconnected ipInfo) {
  Serial.println(F("INFO: Station disconnected from softAP."));
  if(WiFi.softAPgetStationNum()==0) {
    Serial.println(F("INFO: Leave config mode"));
    core.connectionAttempts=0;
    Serial.println (F("WLAN: WiFi.mode(WIFI_STA)"));
    WiFi.mode(WIFI_STA);
  }
}

// Manage network disconnection
void onSTADisconnected (WiFiEventStationModeDisconnected event_info) {
  core.connectionAttempts++;
  Serial.printf_P (PSTR("INFO: Disconnected from SSID: %s\n"), event_info.ssid.c_str ());
  Serial.printf_P (PSTR("INFO: Reason: %d\n"), event_info.reason);
  if( core.connectionAttempts > 10 && millis()<600000) {
    Serial.println(F("WLAN: WiFi.mode(WIFI_AP)"));
    WiFi.mode(WIFI_AP);
    WiFi.softAP( core.cfg.get("name")->getValue() );
  }
}

void statusLED(void) {
  //toggle state
  uint8 state = digitalRead(ONBOARDLED);  // get the current state of GPIO1 pin
  digitalWrite(ONBOARDLED, !state);     // set pin to the opposite state
}

void handleInterrupt1(void) {
  core.counter[0]++;
}

void handleInterrupt2(void) {
  core.counter[1]++;
}

void handleInterrupt3(void) {
  core.counter[2]++;
}

void handleInterrupt4(void) {
  core.counter[3]++;
}
