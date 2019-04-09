#ifndef CORE_H_
#define CORE_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncDNSServer.h>
//#include <asyncHTTPrequest.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include <Dusk2Dawn.h>
#include <Ticker.h>
#include <FS.h>
#include <LuaWrapper.h>
#include <math.h>

#include "webserver.h"
#include "config.h"
#include "plugin.h"
#include "sendemail.h"
#include "plugin.h"
#include "config.h"
#include "update.h"

#include "plugin_ledstrip.h"
#include "plugin_ir.h"
#include "plugin_bme280.h"
//#include "plugin_ssd1306.h"
//#include "plugin_dht.h"
//#include "plugin_mqtt.h"
//#include "plugin_rc.h"
//#include "plugin_icmpping.h"
//#include "plugin_ws2812.h"
#endif
#if defined(ESP32)
//#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFi.h>
#include <SPIFFS.h>
#include <rom/rtc.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncDNSServer.h>
#include <AsyncTCP.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include <Dusk2Dawn.h>
#include <Ticker.h>
#include <FS.h>
#include <LuaWrapper.h>
#include <math.h>

#include "webserver.h"
#include "config.h"
#include "plugin.h"
#include "sendemail.h"
#include "plugin.h"
#include "config.h"
#include "update.h"
#endif

//#define DEBUGMODE
#define ONBOARDLED 2
#define UDPNEWNODEINTERVAL 60000
#define UDPNODETIMEOUT 200000
#define MAXNODES 16
#define RULEINTERVAL 50

typedef struct{
  IPAddress ip;
  char *name;
  unsigned long lastsignal;
}ESPNode;

extern String luaresult;

void onSTAGotIP (WiFiEventStationModeGotIP ipInfo);
void onSTAConnected (WiFiEventStationModeConnected ipInfo);
void onAP_STAConnected (WiFiEventSoftAPModeStationConnected ipInfo);
void onAP_STADisconnected (WiFiEventSoftAPModeStationDisconnected ipInfo);
void onSTADisconnected (WiFiEventStationModeDisconnected event_info);
void handle_FILE_iro (AsyncWebServerRequest *request);
void handle_FILE_w3css (AsyncWebServerRequest *request);
void handleConfig (AsyncWebServerRequest *request);
void handleJSON (AsyncWebServerRequest *request);
void handleControl (AsyncWebServerRequest *request);
void handleUDPPacket(AsyncUDPPacket packet);
void statusLED(void);
void handleInterrupt1(void);
void handleInterrupt2(void);
void handleInterrupt3(void);
void handleInterrupt4(void);

class Core : public Plugin {
  public:
    Core(void);
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    void updateNodes(void);
    void deleteNode(uint8 id);

    AsyncUDP udpClient;
    AsyncWebServer webServer = AsyncWebServer(80);
    AsyncDNSServer dnsServer;
    //asyncHTTPrequest *httpRequest;
    Config cfg; // configuration
    ESPNode *knownESPNodes[MAXNODES]; // known nodes in neigborhood
    unsigned long counter[10]; // counter for interrupts
    unsigned int avrIteration; // avr iterationtime
    Ticker LEDTicker; // LED Ticker
    File _file;
    unsigned int connectionAttempts;
    String serialcommand;
    String command;
    bool _scriptexecute;
  private:
    WiFiEventHandler stationGotIPHandler;
    WiFiEventHandler stationDisconnectedHandler;
    WiFiEventHandler stationConnectedHandler;
    WiFiEventHandler apStationConnectedHandler;
    WiFiEventHandler apStationDisconnectedHandler;
    time_t _timenow;
    time_t _starttime;
    unsigned long _lastudpsend;
    unsigned long _lastRulesRun;
    unsigned long _lastIteration;
    bool _rulesActive;
};

#endif
