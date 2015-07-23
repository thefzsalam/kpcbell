#pragma once

#include <Arduino.h>
#include "TimeProvider.h"

class SoftTimeProvider : public TimeProvider {
  private:
    byte _hour;
    byte _minute;
    byte _sec;
    byte _day;
    byte _month;
    int _year;
    bool _isAM;

  public:
    void setSeconds(byte sec);
    void setMinutes(byte min);
    void setHours(byte hour);
    void setAM(bool isAM);
    void setDay(byte day);
    void setMonth(byte month);
    void setYear(int year);

    //due to the fact that many time libraries only provide
    //a one shot setTime(hr,min,day,year,etc) function
    //this method may need to be called as per implementations
    void flush();

    byte getMinutes();
    byte getHours();
    bool isAM();
    byte getWeekDay();
    byte getDay();
    byte getMonth();
    int  getYear();
    long millis();
};
