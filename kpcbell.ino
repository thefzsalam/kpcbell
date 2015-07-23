#include "SevSeg.h"
#include "SRLeds.h"
#include "SRButtons.h"
#include "ButtonsManager.h"
#include "SoftTimeProvider.h"
#include "UI.h"
#include "MainUI.h"
#include "AlarmsUI.h"
#include "TimeDateSetUI.h"

//Output Elements
byte commonSrPins[4] = {PIN_DISPLAY_COMM_1,PIN_DISPLAY_COMM_2,PIN_DISPLAY_COMM_3,PIN_DISPLAY_COMM_4};
SevSeg sevenSeg(PIN_DISPLAY_CLOCK, PIN_DISPLAY_DATA,commonSrPins,true);
SRLeds weeksDisplay(PIN_WEEKS_DISPLAY_CLOCK, PIN_WEEKS_DISPLAY_DATA);
SRLeds indicatorLeds(PIN_EXTRA_DISPLAY_CLOCK, PIN_EXTRA_DISPLAY_DATA);

//active low. ie when the button is pressed it should be low.
//so that I can use internal pull up to make the button inactive
SRButtons buttons(PIN_BTNS1_DATA,PIN_BTNS1_CLOCK,PIN_BTNS1_INPUT,false); 

//TODO:time provider
TimeProvider timeProvider = SoftTimeProvider();
AlarmProvider alarmProvider = AlarmProvider();
ButtonsManager buttonsManager = ButtonsManager(buttons,timeProvider);

UI mainUI = MainUI(sevenSeg,weeksDisplay,indicatorLeds,timeProvider,alarmProvider);
UI alarmsUI = AlarmsUI(alarmProvider,sevenSeg,indicatorLeds, weeksDisplay, buttonsManager);
UI timeDateSetUI = TimeDateSetUI(timeProvider,buttonsManager,sevenSeg,indicatorLeds);

enum MODE {
  MAIN,ALARMS,TIMEDATESET
};

MODE mode = MAIN;

void setup() {

  pinMode(PIN_DISPLAY_DATA, OUTPUT);
  pinMode(PIN_DISPLAY_CLOCK, OUTPUT);

  pinMode(PIN_WEEKS_DISPLAY_DATA, OUTPUT);
  pinMode(PIN_WEEKS_DISPLAY_CLOCK, OUTPUT);

  pinMode(PIN_EXTRA_DISPLAY_DATA, OUTPUT);
  pinMode(PIN_EXTRA_DISPLAY_CLOCK, OUTPUT);

  pinMode(PIN_BTNS1_DATA, OUTPUT);
  pinMode(PIN_BTNS1_CLOCK, OUTPUT);
  pinMode(PIN_BTNS1_INPUT, INPUT_PULLUP);
  alarmProvider.loadAlarmsFromROM();
}

void loop() {

  switch(mode) {
    case MAIN: 
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

