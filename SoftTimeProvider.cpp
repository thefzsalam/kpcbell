#include <Arduino.h>
#include "SoftTimeProvider.h"
#include "Time.h"

void SoftTimeProvider::setSeconds(byte sec) {
  this->_sec = sec;
}
void SoftTimeProvider::setMinutes(byte min) {
  this->_minute = min;
}
void SoftTimeProvider::setHours(byte hour) {
  this->_hour = hour;
}
void SoftTimeProvider::setAM(bool isAM) {
  this->_isAM = isAM;
}
void SoftTimeProvider::setDay(byte day) {
  this->_day = day;
}
void SoftTimeProvider::setMonth(byte month) {
  this->_month = month;
}
void SoftTimeProvider::setYear(int year) {
  this->_year = year;
}
void SoftTimeProvider::flush() {
  setTime(_hour,_minute,_sec,_day,_month,_year);
}
byte SoftTimeProvider::getMinutes() {
  return _minute = (byte) minute();
}
byte SoftTimeProvider::getHours() {
  return _hour = (byte) hourFormat12();
}
bool SoftTimeProvider::isAM() {
  return _isAM = ::isAM();
}
//Sunday 0, Sat 7
byte SoftTimeProvider::getWeekDay() {
  return (byte)(weekday()-1);
}
byte SoftTimeProvider::getDay() {
  return _day = (byte)day();
}
byte SoftTimeProvider::getMonth() {
  return _month = (byte)month();
}
int SoftTimeProvider::getYear() {
  return _year = year();
}
long SoftTimeProvider::millis() {
  return ::millis();
}

