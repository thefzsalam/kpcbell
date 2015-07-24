#pragma once

#include <Arduino.h>

struct Alarm {
  byte hour;
  byte minute;
  bool isAM;
  byte weekdays;
};
class AlarmProvider{
  protected:
    static const byte EEPROM_CHECK = 0b10101001;
    static const byte MAX_ALARMS_LENGTH = 15;
    byte alarmsLength;
    Alarm alarms[MAX_ALARMS_LENGTH];
   public:
    //AlarmProvider();
    Alarm getNextAlarm(byte hr, byte min,byte weekday);
    Alarm getAlarm(byte b);
    byte getAlarmsLength();
    void addAlarm(Alarm alarm);
    void modifyAlarm(byte index,Alarm alarmUpdated);
    void deleteAlarm(byte index);
    void deleteAlarm(Alarm alarm);
    void saveAlarmsToROM();
    void loadAlarmsFromROM();
   			
};
bool operator== (const Alarm& lhs, const Alarm& rhs);
