#include<Arduino.h>
#include "UI.h"
#include "MainUI.h"

  MainUI::MainUI(SevSeg& sevSeg,
      SRLeds& weeksLed,
      SRLeds& indicator,
      TimeProvider& timeProvider,
      AlarmProvider& alarmProvider) :
    sevSeg(sevSeg),weeksLed(weeksLed), indicators(indicator), timeProvider(timeProvider),
    alarmProvider(alarmProvider)
  {
  }

  void MainUI::turnOnIndicators(bool time,bool data,bool alarm,bool am) {
    indicators.setLedOnOff(LED_TIMENOW,time);
    indicators.setLedOnOff(LED_DATE,data);
    indicators.setLedOnOff(LED_ALARM,alarm);
    indicators.setLedOnOff(LED_AM,time&&am);
    indicators.setLedOnOff(LED_PM,time&&(!am));
  }
  void MainUI::process() {
    int delay=(int)(timeProvider.millis()-lastUpdated);
    if(delay<delayUpdate) return;
    
    switch(currentDisplay) {
      case DISPLAY_TIME_NOW:
	{
	byte hour = timeProvider.getHours();
	bool isAM = hour<12;
	
	if(hour==0) hour=12;
	else if(hour>12) hour-=12;

	byte minutes = timeProvider.getMinutes();
	sevSeg.setAllDigits(hour/10,hour%10,minutes/10,minutes%10);
	weeksLed.setAllLeds(0);
	turnOnIndicators(true,false,false,isAM);	
	currentDisplay=DISPLAY_DATE;
	break;
	}
      case DISPLAY_DATE: 
	{
	byte day = timeProvider.getDay();
	byte month = timeProvider.getMonth();
	byte weekDay = timeProvider.getWeekDay();
	sevSeg.setAllDigits(day/10,day%10,month/10,month%10);
	weeksLed.setAllLeds(0);
	weeksLed.setLedOnOff(weekDay,true);
	turnOnIndicators(false,true,false,false);	
	currentDisplay=DISPLAY_NEXT_ALARM;
	break;
	}
      case DISPLAY_NEXT_ALARM:
	{
	Alarm alarm = alarmProvider.getNextAlarm(timeProvider.getHours(),timeProvider.getMinutes(),timeProvider.getWeekDay());
	sevSeg.setAllDigits(alarm.hour/10,alarm.hour%10,alarm.minute/10,alarm.minute%10);
	weeksLed.setAllLeds(0);
	indicators.setAllLeds(0);
	indicators.setLedOnOff(2,true);
	turnOnIndicators(true,false,false,alarm.isAM);
	currentDisplay=DISPLAY_TIME_NOW;
	break;
	}
     }

     lastUpdated = timeProvider.millis();
  }

  void MainUI::saveData() {}
  void MainUI::initialize() {}
