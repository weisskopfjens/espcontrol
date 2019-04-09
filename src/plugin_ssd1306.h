#ifndef PLUGIN_SSD1306_H_
#define PLUGIN_SSD1306_H_

#include <LuaWrapper.h>
#include <SSD1306Wire.h>
#include <Wire.h>
#include "plugin.h"



class plugin_SSD1306 : public Plugin {
  public:
    plugin_SSD1306(void);
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    SSD1306Wire *display;
  private:

};

//int lua_ssd1306drawString (lua_State *L);

#endif
