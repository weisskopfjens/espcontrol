#ifndef PLUGIN_IR_H_
#define PLUGIN_IR_H_

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include <LuaWrapper.h>
#include "plugin.h"

class plugin_IR : public Plugin {
  public:
    plugin_IR(void);
    int getIRValue(void);
    void setup(void);
    void loop(void);
    void json(AsyncWebServerRequest *request,AsyncResponseStream *response);
    int _irrx_result;
    IRrecv *_irrx;
    IRsend *_irtx;
  private:

};



#endif
