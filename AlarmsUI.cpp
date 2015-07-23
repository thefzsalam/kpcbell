#include <Arduino.h> 
#include "AlarmsUI.h"
#include "Constants.h"
/*
 * Objectives:
 *  Show a list of alarms
 *  navigatable using up and down
 *  at the either end of list,LpAndRel up or down will go to the other end
 *
 *  LpAndRel add button will switch to add mode
 *  pressing up and down will change the digits after validating
 *  SpAndRel Set button will switch to next digit
 *   if digit==4, switch to am-pm, then weekdays
 *   in am-pm mode, up down controls am pm
 *   in weekdays mode, up down to switch weekdays
 *    spAndRel set btn to turn on or off active weekday
 *    inactive weekday leds will be either on or off according to their values
 *    active weekday leds will blink fast if on, blink slow if off
 *    pressing down at the first weekday to go back to digit editing mode
 *    pressing up at the last weekday will also go to digit editing mode
 *  LpAndRel set btn will save the alarm
 *  
 *  LpAndRel set button to enter edit mode
 *  other button controls are same as add mode
 *
 *  LpAndRel delete will enter delete mode
 *  all digits and indicators will blink
 *  delete indicator will stay lit
 *  LpAndRel delete will confirm delete
 *
 *  process loop:
 *    
 *    render digits[4] to sevseg
 *    
 */
  
  AlarmsUI::AlarmsUI(AlarmProvider& alarmProvider, SevSeg& sevSeg,
     SRLeds& indicator,SRLeds& weekdaysLeds,ButtonsManager& buttonsManager) : 
    alarmProvider(alarmProvider), sevSeg(sevSeg),indicator(indicator),
    weekdaysLeds(weekdaysLeds), buttonsManager(buttonsManager){
  }

  ///------------ INITIALIZE THE UI ------------------///
  void AlarmsUI::initialize() {
    totalNoOfAlarms=alarmProvider.getAlarmsLength();
    loadAlarmToDispVars(); //index is automatically 0 or the previous value
    renderDisplay();
  }


  ///------------ UTILITY FUNCTIONS -----------------//
  
  // These functions are re usable functions which 
  // modifies the display variables and do some
  // calculations on it
 
  //verifies weather the given digit data is valid
  //the users of this function are supposed to update the
  //digits[] array themselves if the newdigits[] are valid
  bool AlarmsUI::isValid(byte newDigits[4]) {
    return (newDigits[0]*10+newDigits[1])<=12 && (newDigits[2]*10+newDigits[3])<60;
  }

  //first two: returns first two digits as a number <= 99
  //second two: returns second two digits as a number <= 99
  //both: returns a 4 digit number <=9999
  int AlarmsUI::aggregateDigits(bool firstTwo,bool lastTwo) {
    byte data=0;
    if(firstTwo) data=digits[0]*10+digits[1];
    if(lastTwo) data=data*100 + digits[2]*10 + digits[3];
  }

  //returns an instance of Alarm, populated
  //with our display variables
  Alarm AlarmsUI::alarmFromVars() {
    Alarm a;
    a.hour = aggregateDigits(true,false);
    a.minute = aggregateDigits(false,true);
    a.isAM = isAM;
    a.weekdays = weekdays;
    return a;
  }

  //loads the alarm at activeAlarmIndex from AlarmProvider
  //to the display variables  
  void AlarmsUI::loadAlarmToDispVars() {
    Alarm a = alarmProvider.getAlarm(activeAlarmIndex);
    digits[0] = a.hour/10; digits[1] = a.hour%10;
    digits[2] = a.minute/10; digits[3] = a.minute%10;
    isAM = a.isAM;
    weekdays = a.weekdays;
  }

  //increments activeAlarmIndex and load it to display
  void AlarmsUI::loadNextAlarmToDispVars() {
    activeAlarmIndex=activeAlarmIndex>=totalNoOfAlarms-1?0:activeAlarmIndex+1;
    loadAlarmToDispVars();
  }

  //decrements activeAlarmIndex and load it to display
  void AlarmsUI::loadPrevAlarmToDispVars() {
    activeAlarmIndex=activeAlarmIndex>0?activeAlarmIndex-1:totalNoOfAlarms-1;
    loadAlarmToDispVars();
  }

  
///--------- RENDERING VARIABLES TO DISPLAY FUNCTIONS --------///
  //Loads our display vars to appropriate hardware;
  //                  !MAYCHANGEINFUTURE!
  void AlarmsUI::renderDisplay() {
    weekdaysLeds.setAllLeds(weekdays);
    sevSeg.setAllDigits(digits[0],digits[1],digits[2],digits[3]);
    indicator.setLedOnOff(LED_AM,isAM);
    indicator.setLedOnOff(LED_PM,!isAM);
  }
  
  void AlarmsUI::renderEditWeekdays() {
    weekdaysLeds.setAllLeds(weekdays);
    weekdaysLeds.setAllLedsBlink(0b10000000>>edit_activeWeekday);
    weekdaysLeds.setAllLedsBlinkFast(weekdays&(0b10000000>>edit_activeWeekday));
  }
  ///------- END OF RENDERING VARIABLES TO DISPLAY FUNCTIONS --///
 
  //cycles through submodes
  void AlarmsUI::nextSubMode() {
    switch(subMode){
      case DIGITS:subMode=AM_PM;
      case AM_PM:subMode=WEEKDAYS;
      case WEEKDAYS:subMode=DIGITS;
    }
  }
  void AlarmsUI::initCurrentSubMode() {
    switch(subMode){
      case DIGITS:initSubModeDigits();
      case AM_PM:initSubModeAmPm();
      case WEEKDAYS:initSubModeWeekdays();
    }
  }

  ///---------- INITIALIZE MAIN MODES ----------///
  void AlarmsUI::initAddMode() {
    edit_activeDigit=0; edit_activeWeekday=0;
    digits[0]=1;digits[1]=digits[2]=digits[3]=0;
    weekdays=0b1111000;
    isAM=true;
    indicator.setLedOnOff(LED_ADD,true);
  }

  void AlarmsUI::initEditMode() {
    edit_activeDigit=0; edit_activeWeekday=0;
    loadAlarmToDispVars();
    indicator.setLedOnOff(LED_EDIT,true);
  }
  void AlarmsUI::initDelMode() {
    loadAlarmToDispVars();
    indicator.setLedOnOff(LED_DEL,true);
    sevSeg.setBlink(0,true); sevSeg.setBlink(1,true);
    sevSeg.setBlink(2,true); sevSeg.setBlink(3,true);
  }
  ///------- END OF INITIALIZE MAIN MODES ------///
  
  ///---------- INITIALIZE SUB MODES  ----------///
  void AlarmsUI::initSubModeDigits() {
    sevSeg.setBlink(0,true);
  }
  void AlarmsUI::initSubModeAmPm() {
    indicator.setLedOnOff(LED_AM,isAM);
    indicator.setLedBlink(LED_AM,isAM);
    indicator.setLedOnOff(LED_PM,!isAM);
    indicator.setLedBlink(LED_PM,!isAM);
  }
  void AlarmsUI::initSubModeWeekdays() {
    edit_activeWeekday=0;
    renderEditWeekdays();
  }
  ///------- END OF INITIALIZE SUB MODES  ------///
  

  ///---------- END MAIN MODES  ----------///
  void AlarmsUI::endAddMode(bool save) {
    if(save) {
      Alarm a = alarmFromVars();
      alarmProvider.addAlarm(a);
      activeAlarmIndex = alarmProvider.getAlarmsLength() - 1;
    }
    indicator.setLedOnOff(LED_ADD,false);
    sevSeg.setBlink(0,false);sevSeg.setBlink(1,false);
    sevSeg.setBlink(2,false);sevSeg.setBlink(3,false);
  }

  void AlarmsUI::endEditMode(bool save) {
    if(save) {
      Alarm a = alarmFromVars();
      alarmProvider.modifyAlarm(activeAlarmIndex,a);
      indicator.setLedOnOff(LED_EDIT,false);
    }
    sevSeg.setBlink(0,false);sevSeg.setBlink(1,false);
    sevSeg.setBlink(2,false);sevSeg.setBlink(3,false);
  }

  void AlarmsUI::endDelMode(bool save) {
    if(save) {
      alarmProvider.deleteAlarm(activeAlarmIndex);
      activeAlarmIndex=activeAlarmIndex>0?activeAlarmIndex-1:0;
    }
    sevSeg.setBlink(0,false); sevSeg.setBlink(1,false);
    sevSeg.setBlink(2,false); sevSeg.setBlink(3,false);
    indicator.setLedOnOff(LED_DEL,false);
  }
  ///---------- END OF END MAIN MODES  ----------///

  void AlarmsUI::process() {
     
     switch(mode) {
       case LIST: //DOWN->PrevAlarm; UP->NextAlarm; SET->StartEdit; ADD->StartAdd; DEL->Del;
	 //handle the button press and initialize appropriate modes
	 if(buttonsManager.isSpAndRel(BTN_DOWN)) 
	   loadPrevAlarmToDispVars();
         else if(buttonsManager.isSpAndRel(BTN_UP))
	   loadNextAlarmToDispVars();
	 else if(buttonsManager.isSpAndRel(BTN_SET)) {
   	    mode=EDIT;initEditMode();}
	 else if(buttonsManager.isSpAndRel(BTN_ADD_AL)) {
	   mode=ADD;initAddMode();}
	 else if(buttonsManager.isSpAndRel(BTN_DEL_AL)) {
	   mode=DELETE;initDelMode();}
	 break;

       //add and edit modes have differences only in init and end scripts;
       //init is handled by LIST mode. end script is handled accrondingly using if(mode==..)
       case ADD:
       case EDIT:
	 //pressing sp back will exit adding or editing
	 switch(subMode) {
	   case DIGITS://up->digit++;down->digit--;set->cycleDigits;LPSET->AMPM submode;
	     if(buttonsManager.isSpAndRel(BTN_UP)) {
	       digits[edit_activeDigit]++;
	       if(!isValid(digits))digits[edit_activeDigit]--;
	       else renderDisplay();
	     } else if(buttonsManager.isSpAndRel(BTN_DOWN)) {
	       digits[edit_activeDigit]--;
	       if(!isValid(digits))digits[edit_activeDigit]++;
	       else renderDisplay();
	     } else if(buttonsManager.isSpAndRel(BTN_SET)) {
	       if(edit_activeDigit<3) {
		 sevSeg.setBlink(edit_activeDigit,false);
		 edit_activeDigit++;
		 sevSeg.setBlink(edit_activeDigit,true);
	       }
	       else {
		 sevSeg.setBlink(edit_activeDigit,false);
		 edit_activeDigit=0;
		 sevSeg.setBlink(edit_activeDigit,true);
	       }
	     } else if(buttonsManager.isLpAndRel(BTN_SET)) {
	       nextSubMode(); initCurrentSubMode();
	     }
	     break;
	   case AM_PM:
	     if(buttonsManager.isSpAndRel(BTN_UP)||
		 buttonsManager.isSpAndRel(BTN_DOWN)) {
	       isAM=!isAM;
	       indicator.setLedBlink(LED_AM,isAM);indicator.setLedBlink(LED_PM,!isAM);
	     } else if(buttonsManager.isSpAndRel(BTN_SET)) {
	       nextSubMode(); initCurrentSubMode();
	     }
	     break;
	   case WEEKDAYS:
	     if(buttonsManager.isSpAndRel(BTN_UP)) {
	       edit_activeWeekday=edit_activeWeekday<6?edit_activeWeekday+1:0;      
	       renderEditWeekdays();
	     } else if(buttonsManager.isSpAndRel(BTN_DOWN)) {
	       edit_activeWeekday=edit_activeWeekday>0?edit_activeWeekday-1:6;      
	       renderEditWeekdays();
	     } else if(buttonsManager.isSpAndRel(BTN_SET)) {
	       weekdays ^= 0b10000000>>edit_activeWeekday;
	       renderEditWeekdays();
	     } else if(mode==EDIT&&buttonsManager.isLpAndRel(BTN_SET)) {
	       endEditMode(true);initialize();
	     } else if(mode==ADD&&buttonsManager.isLpAndRel(BTN_ADD_AL)) {
	       endAddMode(true);initialize();
	     } 
	     break;
	 }
	 if(buttonsManager.isSpAndRel(BTN_BACK)) {
	   if(mode==EDIT) {endEditMode(false);initialize();}
	   else if (mode==ADD) {endAddMode(false);initialize();}
	 }
	 break;
       case DELETE:
	 if(buttonsManager.isLpAndRel(BTN_DEL_AL)) {
	  endDelMode(true);initialize();
	 } else if(buttonsManager.isSpAndRel(BTN_BACK)) {
	  endDelMode(false);initialize();
	 }
	 break;
     }
  }
  
