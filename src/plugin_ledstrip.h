#ifndef PLUGIN_LEDSTRIP_H_
#define PLUGIN_LEDSTRIP_H_

#include <Arduino.h>
#include <LuaWrapper.h>
#include "plugin.h"

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct {
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HSV;

RGB HsvToRgb(HSV hsv);
HSV RgbToHsv(RGB rgb);

class plugin_LEDstrip : public Plugin {
public:
  plugin_LEDstrip( void );
  void showAnalogRGB( void );
  void htmlcolor2RGB( String htmlcol, RGB& rgb );
  void RGB2htmlcolor( String &htmlcol, RGB rgb );
  void setLEDcolor( uint8_t h, uint8_t s, uint8_t v );
  HSV getLEDcolor(void);
  void setLEDhue( uint8_t h );
  void setLEDsat( uint8_t s );
  void setLEDval( uint8_t v );
  void chgLEDhue( uint8_t h );
  void chgLEDsat( uint8_t s );
  void chgLEDval( uint8_t v );
  void changeLEDcolor(uint8_t h, uint8_t s, uint8_t v);
  void LEDoff(void);
  void LEDon(void);
  bool isON(void);
  void toggleLED(void);
  void processAnimation(void);
  int  getAnimation(void);
  void animParty(void);
  void animFlash(void);
  void animStrobe(void);
  void animColorcycle(void);
  void animFadeout(void);
  void animFade(void);
  void animSmooth(void);
  void initAnimation(int nr);
  void setAnimationSpeed(uint speed);
  uint getAnimationSpeed(void);
  void setup(void);
  void loop(void);
  void json (AsyncWebServerRequest *request,AsyncResponseStream *response);
  uint8 _rpin,_gpin,_bpin;

private:
  HSV current_hsv;
  HSV saved_hsv;
  RGB old_rgb;
  bool LEDsON;
  uint8_t LEDmode;
  uint8_t animmode;
  unsigned long animi;
  unsigned long _lastcall;
  unsigned int _animationspeed;
};



#endif
