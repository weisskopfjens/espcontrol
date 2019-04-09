#ifndef MISC_H_
#define MISC_H_

#include <Arduino.h>
#include <TimeLib.h>

int min2DayHour (int minutes);
int min2DayMinutes (int minutes);
bool splitTime (int &h,int &m,int &s,String time);
int minOfDay(time_t timevar);

#endif
