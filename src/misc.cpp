#include "misc.h"

int min2DayHour (int minutes) {
  return minutes / 60;
}

int min2DayMinutes (int minutes) {
  return minutes % 60;
}

bool splitTime (int &h,int &m,int &s,String time) {
  sscanf(time.c_str(),"%d:%d:%d",&h,&m,&s);
  return true;
}

int minOfDay(time_t timevar) {
  return hour(timevar)*60+minute(timevar);
}
