#include "AlarmProvider.h"
#include "EEPROM2.h"


  bool operator== (const Alarm& lhs,const Alarm& rhs) {
    return (lhs.hour==rhs.hour) &&
	   (lhs.minute == rhs.minute) && 
	   (lhs.weekdays == rhs.weekdays) ;
  }
  Alarm AlarmProvider::getNextAlarm(byte hr, byte min,byte weekday) {
    weekday=0b10000000>>weekday;
    Alarm al={0,0};
    int delta=60*24; //set delta to maximum possible value
    for(byte i=0; i<alarmsLength; i++) {
      if(
	  alarms[i].weekdays&weekday && //ands the weekday flag and check != 0b00000000
	  (alarms[i].hour*60+alarms[i].minute)>(hr*60+min) //whether the alarm is after now
      ) {
	if(al==(Alarm{0,0})) al=alarms[i]; //if al is not init'ed,al is the first found alarm;
	else {
	  //find the difference in mins btwn now and alarm time
	  int d2 = (alarms[i].hour*60+alarms[i].minute) - (hr*60+min);
	  if(d2<delta) { //check the last alarms diff is less than current
	    delta=d2;
	    al = alarms[i];
	  }
	}
      }//if1 : weekday, after now
    }//for loop
    return al;
  }
  Alarm AlarmProvider::getAlarm(byte index) {
    return alarms[index];
  }

  byte AlarmProvider::getAlarmsLength() {
    return alarmsLength;
  }

  void AlarmProvider::addAlarm(Alarm alarm) {
    if(alarmsLength>=MAX_ALARMS_LENGTH) return;
    else {
      alarms[alarmsLength] = alarm;
      alarmsLength++;
    }
  }

  void AlarmProvider::deleteAlarm(Alarm alarm) {
    //bytes can't be -1 (they're unsigned)
    //so alarmsLength, ie lastAlarmIndex + 1 is
    //chosen as a default value instead of -1
    //to indicate not found, error etc
    byte toDel=alarmsLength; 

    byte i;
    for(i=0;i<alarmsLength;i++) {
      //the == operator is overriden
      if(alarms[i]==alarm) {
	//if alarms[i] is the 'alarm' we're looking for,
	//store its index and break the loop
	toDel = i;
	break;
      }
    }

    if(toDel<alarmsLength) { //whether toDel is found
      //pass on the index to delete
      //to overloaded delete function
      deleteAlarm(toDel);
    }
  }  

  void AlarmProvider::deleteAlarm(byte index) {
    //reduce the alarmsLength by one
    alarmsLength--;
    //starting from index, until (old alarmsLength)-1,
    //set i'th element to (i+1)th element,
    //effectively overwriting the first element
    //after this last element and (last-1)th element is same
    //so we've already decremented alarmsLength, effectively hiding the last element
    byte i;
    for(i=index;i<alarmsLength;i++) {
      alarms[i]=alarms[i+1];
    }
  }

  void AlarmProvider::modifyAlarm(byte index,Alarm alarmUpdated) {
    alarms[index]=alarmUpdated;
  }
 
  //checks for alarms stored in EEPROM
  void AlarmProvider::loadAlarmsFromROM() {
    byte eeprom_check_read;
    int romAddress = 0;
    EEPROM.get(romAddress, eeprom_check_read);
    romAddress += sizeof(eeprom_check_read);
    //to check whether any valid data is present in the eeprom
    //define a constant and check whether it exists in the eeprom's first few bytes
    if (eeprom_check_read != EEPROM_CHECK) { 
      //EEPROM_CHECK is supposed to be defined in the header file
      //it is fixed random byte
      alarmsLength = 0;
      return;
    }

    EEPROM.get(romAddress, alarmsLength);
    romAddress += sizeof(alarmsLength);
    if (alarmsLength > 15) {
      //alarmsLength is not supposed to be >15
      //this means data is corrupt
      alarmsLength = 0;
      return;
    }

    int alarmSize = sizeof(Alarm);
    for (int i = 0; i < alarmsLength; i++) {
      EEPROM.get(romAddress, alarms[i]);
      romAddress += alarmSize;
    }
  }

  void AlarmProvider::saveAlarmsToROM() {
    int romAddress = 0;
    byte check = EEPROM_CHECK;
    EEPROM.put(romAddress,check);
    romAddress += sizeof(check);
    //to check whether any valid data is present in the eeprom
    //define a constant and check whether it exists in the eeprom's first few bytes

    EEPROM.put(romAddress, alarmsLength);
    romAddress += sizeof(alarmsLength);

    int alarmSize = sizeof(Alarm);
    for (int i = 0; i < alarmsLength; i++) {
      EEPROM.put(romAddress, alarms[i]);
      romAddress += alarmSize;
    }
  }


  
