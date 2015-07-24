#include <Arduino.h>
#include "TimeProvider.h"
#include "RTClib.h"

class RTCTimeProvider : public TimeProvider {

  private:
    RTC_DS1307& rtc;

  public:

    RTCTimeProvider(RTC_DS1307& rtc);
    
    void setSeconds(byte sec);
    void setMinutes(byte min);
    void setHours(byte hour);
    void setDay(byte day);
    void setMonth(byte month);
    void setYear(int year);

    void flush();

    byte getSeconds();
    byte getMinutes();
    byte getHours();
    byte getWeekDay();
    byte getDay();
    byte getMonth();
    int getYear();
    long millis();
};
