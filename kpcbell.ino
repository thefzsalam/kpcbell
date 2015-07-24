#include "Wire.h"
#include "SevSeg.h"
#include "SRLeds.h"
#include "SRButtons.h"
#include "ButtonsManager.h"
#include "TimeProvider.h"
#include "RTCTimeProvider.h"
#include "UI.h"
#include "MainUI.h"
#include "AlarmsUI.h"
#include "TimeDateSetUI.h"
#include "AlarmProvider.h"

//Output Elements
byte commonSrPins[4] = {PIN_DISPLAY_COMM_1,PIN_DISPLAY_COMM_2,PIN_DISPLAY_COMM_3,PIN_DISPLAY_COMM_4};
SevSeg sevenSeg(PIN_DISPLAY_CLOCK, PIN_DISPLAY_DATA,commonSrPins,true);
SRLeds weeksDisplay(PIN_WEEKS_DISPLAY_CLOCK, PIN_WEEKS_DISPLAY_DATA);
SRLeds indicatorLeds(PIN_EXTRA_DISPLAY_CLOCK, PIN_EXTRA_DISPLAY_DATA);

//active low. ie when the button is pressed it should be low.
//so that I can use internal pull up to make the button inactive
SRButtons buttons(PIN_BTNS1_DATA,PIN_BTNS1_CLOCK,PIN_BTNS1_INPUT,false); 

//TODO:time provider
RTC_DS1307 rtc;

RTCTimeProvider timeProvider(rtc);
AlarmProvider alarmProvider;
ButtonsManager buttonsManager(buttons,timeProvider);

MainUI mainUI = MainUI(sevenSeg,weeksDisplay,indicatorLeds,timeProvider,alarmProvider);
AlarmsUI alarmsUI = AlarmsUI(alarmProvider,sevenSeg,indicatorLeds, weeksDisplay, buttonsManager);
TimeDateSetUI timeDateSetUI = TimeDateSetUI(timeProvider,buttonsManager,sevenSeg,indicatorLeds);

enum MODE {
  MAIN,ALARMS,TIMEDATESET
};


MODE mode = MAIN;

bool alarmActive = true;

Alarm nextAlarm;
bool isAlarmRinging=false;
unsigned long alarmRingingSince=0;
byte alarmRingTime = 1500;//ms

void setup() {
  Wire.begin();
  rtc.begin();

  pinMode(PIN_DISPLAY_DATA, OUTPUT);
  pinMode(PIN_DISPLAY_CLOCK, OUTPUT);

  pinMode(PIN_WEEKS_DISPLAY_DATA, OUTPUT);
  pinMode(PIN_WEEKS_DISPLAY_CLOCK, OUTPUT);

  pinMode(PIN_EXTRA_DISPLAY_DATA, OUTPUT);
  pinMode(PIN_EXTRA_DISPLAY_CLOCK, OUTPUT);

  pinMode(PIN_BTNS1_DATA, OUTPUT);
  pinMode(PIN_BTNS1_CLOCK, OUTPUT);
  pinMode(PIN_BTNS1_INPUT, INPUT_PULLUP);
  
  pinMode(PIN_LED_ALARMON,OUTPUT);
  digitalWrite(PIN_LED_ALARMON,alarmActive?HIGH:LOW);
  
  pinMode(PIN_ALARM_OUTPUT,OUTPUT);
  digitalWrite(PIN_ALARM_OUTPUT,LOW);

  alarmProvider.loadAlarmsFromROM();

  nextAlarm = alarmProvider.getNextAlarm(rtc.now().hour(),rtc.now().minute(),rtc.now().dayOfWeek());
}

Alarm tempNextAlarm;
void loop() {
  if(buttonsManager.isSpAndRel(BTN_ALARMON)) {
    alarmActive=!alarmActive;
    digitalWrite(PIN_LED_ALARMON,alarmActive?HIGH:LOW);
  }
  if(isAlarmRinging&&millis()-alarmRingingSince>alarmRingTime){
    digitalWrite(PIN_ALARM_OUTPUT,LOW);
    isAlarmRinging=false;
    alarmRingingSince=0;
  }

  switch(mode) {
    case MAIN: 
      //CHECK FOR ALARM TIME
      tempNextAlarm = alarmProvider.getNextAlarm(rtc.now().hour(),rtc.now().minute(),rtc.now().dayOfWeek());
      if(alarmActive&&!(nextAlarm==tempNextAlarm)) {
	nextAlarm=tempNextAlarm;
	digitalWrite(PIN_ALARM_OUTPUT,HIGH);
	isAlarmRinging=true;
	alarmRingingSince=millis();
      }
      mainUI.process();
      if(buttonsManager.isSpAndRel(BTN_ALARMS)) {
	mode = ALARMS;
	alarmsUI.initialize();
      } else if(buttonsManager.isLpAndRel(BTN_SET)) {
	mode = TIMEDATESET;
	timeDateSetUI.initialize();
      }
      break;
    case ALARMS:
      alarmsUI.process();
      if(buttonsManager.isLpAndRel(BTN_BACK)) {
	alarmsUI.saveData();
	alarmProvider.saveAlarmsToROM();
	mode = MAIN;
	nextAlarm = alarmProvider.getNextAlarm(rtc.now().hour(),rtc.now().minute(),rtc.now().dayOfWeek());
      }
      break;
    case TIMEDATESET:
      timeDateSetUI.process();
      if(buttonsManager.isLpAndRel(BTN_BACK)) {
	timeDateSetUI.saveData();
	mode = MAIN;
      }
      break;
  }

  ///////////////////
  sevenSeg.process();
  weeksDisplay.process();
  indicatorLeds.process();
  //buttons.process();
  buttonsManager.process();
  ///////////////////
}

