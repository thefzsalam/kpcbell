#include <Arduino.h>
#include "Constants.h"

#include "TimeProvider.h"
#include "ButtonsManager.h"
#include "SevSeg.h"
#include "TimeDateSetUI.h"

  TimeDateSetUI::TimeDateSetUI(TimeProvider& timeProvider,
			       ButtonsManager& buttonsManager,
			       SevSeg& sevSeg,
			       SRLeds& indicator):
  timeProvider(timeProvider), buttonsManager(buttonsManager),
  sevSeg(sevSeg), indicators(indicator){
    this->currentEdit = EDIT_TIME;
  }

  //first two: returns first two digits as a number <= 99
  //second two: returns second two digits as a number <= 99
  //both: returns a 4 digit number <=9999
  int TimeDateSetUI::aggregateDigits(bool firstTwo,bool lastTwo) {
    byte data=0;
    if(firstTwo) data=digits[0]*10+digits[1];
    if(lastTwo) data=data*100 + digits[2]*10 + digits[3];
  }
  //get current time and set the digits
  void TimeDateSetUI::initEditTime() {
    byte hour = timeProvider.getHours();
    if(hour==0) hour=12;
    else if(hour>12) hour-=12;
    byte minutes = timeProvider.getMinutes();
    digits[0] = hour/10; digits[1] = hour%10;
    digits[2] = minutes/10; digits[3] = minutes%10;
  }
  //set is am pm and set the led variable
  //the process loop will take care of lighting appropriate
  //indicator led
  void TimeDateSetUI::initEditAM_PM() {
    isAM = timeProvider.getHours()>=12;
  }
  //get day,month and set the digits
  void TimeDateSetUI::initEditDate() {
    byte day = timeProvider.getDay();
    byte month = timeProvider.getMonth();
    digits[0] = day/10; digits[1] = day%10;
    digits[2] = month/10; digits[3] = month%10;
  }
  //get year and set the digits
  void TimeDateSetUI::initEditYear() {
    int year = timeProvider.getYear();
    digits[0] = year/1000; digits[1] = (year/100)%10;
    digits[2] = (year/10)%10; digits[3] = year%10;
  }
  //save time to timeprovider
  void TimeDateSetUI::saveTime() {
    byte hr = (byte)aggregateDigits(true,false);
    //if the current rtc time is pm, then keep the pm
    //the next stage am-pm adjustment will correct it if needed
    if(timeProvider.getHours()>12) hr+=12;   
    timeProvider.setHours(hr);
    timeProvider.setMinutes((byte)aggregateDigits(false,true));
  }
  //save is am_pm to timeprovider
  void TimeDateSetUI::saveAM_PM() {
    byte hrs = timeProvider.getHours();
    if(hrs<=11&&(!isAM)) hrs+=12;
    else if(hrs>=12&&(isAM)) hrs-=12;
    timeProvider.setHours(hrs);
  }
  //save date to timeprovider
  void TimeDateSetUI::saveDate() {
    timeProvider.setDay((byte)aggregateDigits(true,false));
    timeProvider.setMonth((byte)aggregateDigits(false,true));
  }
  //save year to timeprovider
  void TimeDateSetUI::saveYear() {
    timeProvider.setYear(aggregateDigits(true,true));
    timeProvider.flush();
  }

  //verifies weather the given digit data is valid
  //according to the current edit mode
  //the users of this function are supposed to update the
  //digits[] array themselves if the newdigits[] are valid
  bool TimeDateSetUI::isValid(byte newDigits[4]) {
    switch(currentEdit) {
      case EDIT_TIME:
	return (newDigits[0]*10+newDigits[1])<=12 && (newDigits[2]*10+newDigits[3])<60;
      case EDIT_DATE:
	byte month = newDigits[2]*10+newDigits[3];
	byte date  = newDigits[0]*10+newDigits[1];
	if(month>12)return false;
	switch(month) {
	  case 1: case 3: case 5: case 7: case 8: case 10: case 12:
	    return date<=31;
	  case 4: case 6: case 9: case 11: return date<=30;
	  case 2: return date<=29;
	 }
	return false;
    }
    return true;
  }

  //cycles through edit modes
  void TimeDateSetUI::nextMode() {
    switch(currentEdit) {
      case EDIT_TIME: currentEdit=EDIT_AM_PM; break;
      case EDIT_AM_PM: currentEdit=EDIT_DATE; break;
      case EDIT_DATE: currentEdit = EDIT_YEAR; break;
      case EDIT_YEAR: currentEdit = EDIT_TIME; break;
    }
  }
  //call the appropriate initmethod according to current mode variable
  void TimeDateSetUI::initCurrentMode() {
    switch(currentEdit) {
      case EDIT_TIME: initEditTime();break;
      case EDIT_AM_PM: initEditAM_PM();break;
      case EDIT_DATE: initEditDate();break;
      case EDIT_YEAR: initEditYear();break;
    }
  }
  //call the appropriate savemethod according to current mode variable
  void TimeDateSetUI::saveCurrentMode() {
    switch(currentEdit) {
      case EDIT_TIME: saveTime();break;
      case EDIT_AM_PM: saveAM_PM();break;
      case EDIT_DATE: saveDate();break;
      case EDIT_YEAR: saveYear();break;
    }
  }

  //initializes current UI
  void TimeDateSetUI::initialize() {
    initCurrentMode();
  }

  //the process loop! heart of the ui.
  void TimeDateSetUI::process() {
    if(currentEdit==EDIT_AM_PM) {
        //if currently editing am_pm, light the 
	//appropriate indicator leds
	indicators.setLedOnOff(LED_AM,isAM);
	indicators.setLedOnOff(LED_PM,!isAM);
	indicators.setLedBlink(LED_AM,isAM);
	indicators.setLedBlink(LED_PM,!isAM);
    } else {
      //if not editing am_pm, set the digits
      //on sevseg according to the variable
      //blink the activeDigit'th digit
      sevSeg.setBlink(activeDigit,true);

      if(buttonsManager.isSpAndRel(BTN_UP)) {
	digits[activeDigit]++;
	if(!isValid(digits)) digits[activeDigit]--;
	else sevSeg.setAllDigits(digits[0],digits[1],digits[2],digits[3]);
      } else if(buttonsManager.isSpAndRel(BTN_DOWN)) {
	digits[activeDigit]--;
	if(!isValid(digits)) digits[activeDigit]++;
	else sevSeg.setAllDigits(digits[0],digits[1],digits[2],digits[3]);
      }
    }
    //whatever the currentEditMode is,
    //if button_set is SpAndRel
    //edit the next digit
    if(buttonsManager.isSpAndRel(BTN_SET)) {
      sevSeg.setBlink(activeDigit,false);
      activeDigit++;
      if(activeDigit>4) {
	activeDigit=0;
      }
    }
    //whatever the currentMode is
    //if button_set is LpAndRel
    //switch to the next edit mode
    if(buttonsManager.isLpAndRel(BTN_SET)) {
	saveCurrentMode(); //save current mode
	nextMode(); //switch to next mode
	initCurrentMode(); //initialize the new mode
    }
  }
  void TimeDateSetUI::saveData() {
    saveCurrentMode();
  }
