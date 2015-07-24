#include <Arduino.h>
#include "TimeProvider.h"
#include "RTCTimeProvider.h"
#include "RTClib.h"

  RTCTimeProvider::RTCTimeProvider(RTC_DS1307& rtc) : rtc(rtc) {}

  void RTCTimeProvider::setSeconds(byte secs) {
    rtc.adjust(DateTime(getYear(),getMonth(),getDay(), getHours(),getMinutes(),secs));
  }
  void RTCTimeProvider::setMinutes(byte min) {
    rtc.adjust(DateTime(getYear(),getMonth(),getDay(), getHours(),min,getSeconds()));
  }
  void RTCTimeProvider::setHours(byte hrs) {
    rtc.adjust(DateTime(getYear(),getMonth(),getDay(), hrs,getMinutes(),getSeconds()));
  }
   void RTCTimeProvider::setDay(byte day) {
    rtc.adjust(DateTime(getYear(),getMonth(),day, getHours(),getMinutes(),getSeconds()));
  }
  void RTCTimeProvider::setMonth(byte month) {
    rtc.adjust(DateTime(getYear(),month,getDay(), getHours(),getMinutes(),getSeconds()));
  }
  void RTCTimeProvider::setYear(int yr) {
    rtc.adjust(DateTime(yr,getMonth(),getDay(), getHours(),getMinutes(),getSeconds()));
  }

  void RTCTimeProvider::flush() {}

  byte RTCTimeProvider::getSeconds() {
    return rtc.now().second();
  }

  byte RTCTimeProvider::getMinutes() {
    return rtc.now().minute();
  }
  byte RTCTimeProvider::getHours() {
    return rtc.now().hour();
  }
  
 byte RTCTimeProvider::getWeekDay() {
    return rtc.now().dayOfWeek(); //0 is sunday; 
  }
  byte RTCTimeProvider::getDay() {
    return rtc.now().day();
  }
  byte RTCTimeProvider::getMonth() {
    return rtc.now().month();
  }
  int RTCTimeProvider::getYear() {
    return rtc.now().year();
  }
  long RTCTimeProvider::millis() {
    return ::millis();
  }
