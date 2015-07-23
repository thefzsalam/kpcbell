#pragma once

#include <Arduino.h>

class TimeProvider {
  public:
    virtual void setMinutes(byte min);
    virtual void setHours(byte hour);
    virtual void setAM(bool isAM);
    virtual void setWeekDay(byte weekday);
    virtual void setDay(byte day);
    virtual void setMonth(byte month);
    virtual void setYear(int year);

    //due to the fact that many time libraries only provide
    //a one shot setTime(hr,min,day,year,etc) function
    virtual void flush();


    virtual byte getMinutes();
    virtual byte getHours();
    virtual bool isAM();
    virtual byte getWeekDay();
    virtual byte getDay();
    virtual byte getMonth();
    virtual int  getYear();
    virtual long millis();
};
