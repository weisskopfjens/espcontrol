#include "plugin_ledstrip.h"

extern plugin_LEDstrip ledsPlugin;
extern LuaWrapper lua;

int lua_changeLEDcolor (lua_State *L) {
  uint8_t h = luaL_checkinteger(L, 1);  /* get argument */
  uint8_t s = luaL_checkinteger(L, 2);
  uint8_t v = luaL_checkinteger(L, 3);
  ledsPlugin.changeLEDcolor( h, s, v );
  //lua_pushnumber(L, sin(d));  /* push result */
  return 0;  /* number of results */
}

int lua_getAnimationSpeed (lua_State *L) {
  lua_pushinteger(L, ledsPlugin.getAnimationSpeed());
  return 1;
}

int lua_setAnimationSpeed (lua_State *L) {
  uint8_t s = luaL_checkinteger(L, 1);
  ledsPlugin.setAnimationSpeed(s);
  return 0;
}

int lua_setLEDcolor (lua_State *L) {
  uint8_t h = luaL_checkinteger(L, 1);  /* get argument */
  uint8_t s = luaL_checkinteger(L, 2);
  uint8_t v = luaL_checkinteger(L, 3);
  ledsPlugin.setLEDcolor( h, s, v );
  return 0;  /* number of results */
}

int lua_LEDon (lua_State *L) {
  ledsPlugin.LEDon();
  return 0;
}

int lua_LEDoff (lua_State *L) {
  ledsPlugin.LEDoff();
  return 0;
}

int lua_toggleLED (lua_State *L) {
  ledsPlugin.toggleLED();
  return 0;
}

int lua_setLEDMode (lua_State *L) {
  uint8_t anim = lua_tointeger(L, 1);
  ledsPlugin.initAnimation(anim);
  return 0;
}

int lua_getLEDMode (lua_State *L) {
  lua_pushinteger(L, ledsPlugin.getAnimation());  /* push result */
  return 1;
}

int lua_isLEDon (lua_State *L) {
  int on = ledsPlugin.isON();
  lua_pushinteger(L, on);  /* push result */
  return 1;  /* number of results */
}

int lua_getLEDcolor (lua_State *L) {
  HSV temp = ledsPlugin.getLEDcolor();
  lua_pushinteger(L, temp.h);
  lua_pushinteger(L, temp.s);
  lua_pushinteger(L, temp.v);
  return 3;
}

int lua_initLED (lua_State *L) {
  ledsPlugin._rpin = luaL_checkinteger(L, 1);
  ledsPlugin._gpin = luaL_checkinteger(L, 2);
  ledsPlugin._bpin = luaL_checkinteger(L, 3);
  lua.Lua_register(F("setLEDColor"), (const lua_CFunction) &lua_setLEDcolor);
  lua.Lua_register(F("LEDon"), (const lua_CFunction) &lua_LEDon);
  lua.Lua_register(F("LEDoff"), (const lua_CFunction) &lua_LEDoff);
  lua.Lua_register(F("toggleLED"), (const lua_CFunction) &lua_toggleLED);
  lua.Lua_register(F("setLEDMode"), (const lua_CFunction) &lua_setLEDMode);
  lua.Lua_register(F("getLEDMode"), (const lua_CFunction) &lua_getLEDMode);
  lua.Lua_register(F("isLEDOn"), (const lua_CFunction) &lua_isLEDon);
  lua.Lua_register(F("getLEDColor"), (const lua_CFunction) &lua_getLEDcolor);
  lua.Lua_register(F("setAnimationSpeed"), (const lua_CFunction) &lua_setAnimationSpeed);
  lua.Lua_register(F("getAnimationSpeed"), (const lua_CFunction) &lua_getAnimationSpeed);
  lua.Lua_register(F("changeLEDcolor"), (const lua_CFunction) &lua_changeLEDcolor);
  return 0;
}


plugin_LEDstrip::plugin_LEDstrip( void ) {

}

void plugin_LEDstrip::showAnalogRGB( void ) {
  RGB rgb;
  rgb = HsvToRgb(current_hsv);
  if (rgb.r!=old_rgb.r||rgb.g!=old_rgb.g||rgb.b!=old_rgb.b) {
    analogWrite(_rpin, rgb.r*4 );
    analogWrite(_gpin, rgb.g*4 );
    analogWrite(_bpin, rgb.b*4 );
    old_rgb=rgb;
  }
}

void plugin_LEDstrip::htmlcolor2RGB( String htmlcol, RGB& rgb ) {
  String rs, gs, bs;
  htmlcol.replace("#","");
  htmlcol.replace("%23","");
  if(htmlcol.length()==6) {
    rs = "0x" + htmlcol.substring(0, 2);
    gs = "0x" + htmlcol.substring(2, 4);
    bs = "0x" + htmlcol.substring(4, 6);
    rgb.r = (uint8_t)(strtol(rs.c_str(), NULL, 0));
    rgb.g = (uint8_t)(strtol(gs.c_str(), NULL, 0));
    rgb.b = (uint8_t)(strtol(bs.c_str(), NULL, 0));
    return;
  }
  if(htmlcol.length()==3) {
    rs = "0x" + htmlcol.substring(0, 1);
    gs = "0x" + htmlcol.substring(1, 2);
    bs = "0x" + htmlcol.substring(2, 3);
    rgb.r = (uint8_t)(strtol(rs.c_str(), NULL, 0))*17;
    rgb.g = (uint8_t)(strtol(gs.c_str(), NULL, 0))*17;
    rgb.b = (uint8_t)(strtol(bs.c_str(), NULL, 0))*17;
    return;
  }
  Serial.println(F("ERROR: Converting html color rgb"));
}

void plugin_LEDstrip::RGB2htmlcolor( String &htmlcol, RGB rgb ) {
  char temps[10];
  sprintf(temps,"#%02x%02x%02x",rgb.r,rgb.g,rgb.b);
	htmlcol = temps;
}

void plugin_LEDstrip::setLEDcolor( uint8_t h, uint8_t s, uint8_t v ) {
  current_hsv.h = h;
  current_hsv.s = s;
  current_hsv.v = v;
}

HSV plugin_LEDstrip::getLEDcolor(void) {
  return current_hsv;
}

void plugin_LEDstrip::setLEDhue( uint8_t h ) {
  current_hsv.h = h;
}

void plugin_LEDstrip::setLEDsat( uint8_t s ) {
  current_hsv.s = s;
}

void plugin_LEDstrip::setLEDval( uint8_t v ) {
  current_hsv.v = v;
}

void plugin_LEDstrip::chgLEDhue( uint8_t h ) {
  current_hsv.h += h;
}

void plugin_LEDstrip::chgLEDsat( uint8_t s ) {
  current_hsv.s += s;
}

void plugin_LEDstrip::chgLEDval( uint8_t v ) {
  current_hsv.v += v;
}

void plugin_LEDstrip::changeLEDcolor(uint8_t h, uint8_t s, uint8_t v) {
  current_hsv.h += h;
  chgLEDsat( s );
  chgLEDval( v );
}

void plugin_LEDstrip::LEDoff(void) {
  if( isON() == true && animmode==0 ) {
    saved_hsv.h=current_hsv.h;
    saved_hsv.s=current_hsv.s;
    saved_hsv.v=current_hsv.v;
    setLEDval( 0 );
  }
  animmode=0;
  animi=0;
}

void plugin_LEDstrip::LEDon(void) {
  if( isON()==false ) {
    current_hsv.h=saved_hsv.h;
    current_hsv.s=saved_hsv.s;
    current_hsv.v=saved_hsv.v;
  }
}

void plugin_LEDstrip::toggleLED(void) {
  if( isON() ) {
    LEDoff();
  } else {
    LEDon();
  }
}

bool plugin_LEDstrip::isON(void) {
  RGB rgb=HsvToRgb(current_hsv);
  if(rgb.r==0 && rgb.g==0 && rgb.b==0 ) {
    return false;
  } else {
	  return true;
  }
}

void plugin_LEDstrip::initAnimation(int nr) {
  if( nr>0 ){
    if(animmode==0 && isON() ){
      saved_hsv.h=current_hsv.h;
      saved_hsv.s=current_hsv.s;
      saved_hsv.v=current_hsv.v;
    }
    if(nr!=animmode) {
      animi=0;
      animmode=nr;
    }
  } else if( nr==0 && animmode>0 ) {
    animi=0;
    animmode=nr;
    current_hsv.h=saved_hsv.h;
    current_hsv.s=saved_hsv.s;
    current_hsv.v=saved_hsv.v;
  }
}

int plugin_LEDstrip::getAnimation(void) {
  return animmode;
}

void plugin_LEDstrip::processAnimation(void) {
  switch(animmode) {
    case 0:
      return;
    break;
    case 1:
      animFlash();
    break;
    case 2:
      animStrobe();
    break;
    case 3:
      animFade();
    break;
    case 4:
      animSmooth();
    break;
    case 5:
      animParty();
    break;
    case 6:
      animFadeout();
    break;
  }
}

void plugin_LEDstrip::animParty(void) {
  if (animi > 10) {
    animi = 0;
    current_hsv.h = random(255);
  } else {
    animi++;
  }
}

void plugin_LEDstrip::animFlash(void) {
  if (animi<=4) {
    current_hsv.v = 0;
  } else {
    current_hsv.v = 255;
  }
  if (animi > 9) {
    animi = 0;
  } else {
    animi++;
  }
}

void plugin_LEDstrip::animStrobe(void) {
  if (animi<=2) {
    current_hsv.v = 255;
  } else {
    current_hsv.v = 0;
  }
  if (animi > 9) {
    animi = 0;
  } else {
    animi++;
  }
}

void plugin_LEDstrip::animFadeout(void) {
  int temp = ((int)current_hsv.v-1);
  if( temp<=0 ) {
    current_hsv.v = 0;
    //this->saved_hsv = current_hsv;
    animmode = 0;
  } else {
    current_hsv.v-=1;
  }

}

void plugin_LEDstrip::animFade(void) {
  if (animi==0||animi>=510) {
    current_hsv.v = 0;
    current_hsv.h = random(255);
    animi = 0;
  }
  if (animi<=255){
    current_hsv.v=animi;
  } else {
    current_hsv.v--;
  }
  animi++;
}

void plugin_LEDstrip::animSmooth(void) {
  current_hsv.h+=1;
}

void plugin_LEDstrip::setAnimationSpeed(uint speed=50) {
  _animationspeed=speed;
}

uint plugin_LEDstrip::getAnimationSpeed(void) {
  return _animationspeed;
}

void plugin_LEDstrip::setup(void) {
  lua.Lua_register(F("initLED"), (const lua_CFunction) &lua_initLED);
  Serial.println(F("INFO: setup plugin_LEDstrip"));
}

void plugin_LEDstrip::loop(void) {
  if(millis() >= _lastcall+(256-_animationspeed)) {
    _lastcall=millis();
    showAnalogRGB();
    processAnimation();
  }
}

void plugin_LEDstrip::json (AsyncWebServerRequest *request,AsyncResponseStream *response) {
  if (request->hasParam("leds")) {
  	HSV tempc=getLEDcolor();
    response->print(F("{\"leds\":"));
  	response->print(F("{"));
  	response->print(F("\"animation\":\""));
    response->print((String)getAnimation());
  	response->print(F("\",\"animationspeed\":\""));
    response->print((String)getAnimationSpeed());
  	response->print(F("\",\"status\":\""));
    response->print((String)isON());
    response->print(F("\",\"h\":\""));
    response->print((String)tempc.h);
    response->print(F("\",\"s\":\""));
    response->print((String)tempc.s);
    response->print(F("\",\"v\":\""));
    response->print((String)tempc.v);
	  response->print(F("\"}}"));
  }
}

RGB HsvToRgb(HSV hsv) {
    RGB rgb;
    unsigned char region, remainder, p, q, t;
    if (hsv.s == 0) {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }
    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;
    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;
    switch (region) {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }
    return rgb;
}

HSV RgbToHsv(RGB rgb) {
    HSV hsv;
    unsigned char rgbMin, rgbMax;
    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
    hsv.v = rgbMax;
    if (hsv.v == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }
    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0) {
        hsv.h = 0;
        return hsv;
    }
    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);
    return hsv;
}
