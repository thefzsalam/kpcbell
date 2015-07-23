#pragma once

#include <Arduino.h>
#include "UI.h"
#include "AlarmProvider.h"
#include "SevSeg.h"
#include "SRLeds.h"
#include "ButtonsManager.h"
/*
 * Buttons:
 *  0-UP; 1-DOWN; 2-SET; 3-DEL_AL; 4-ADD_AL; 5-ALARMS; 6-BACK;
 * Extra LEDs:
 *  0-TimeNow; 1-Date 2-NextAlarm 3-Change 4-Delete 5-Add 6-AM 7-PM
 * EDIT:
 *  Indicator LED 2 now reads Alarm
 *  When blinking it refers to next alarm
 *  When on and not blinking, it refers to alarms list
 */


class AlarmsUI: public UI {

  private:
    
    enum MODE {
      LIST,ADD,EDIT,DELETE
    };
    enum SUB_MODE {
      DIGITS,AM_PM,WEEKDAYS,NONE
    };
    //keeps track of current mode
    MODE mode = LIST;
    //keeps track of current sub mode 
    //valid for add and edit modes only
    SUB_MODE subMode = DIGITS;

    //the following variables represents
    //the current state of 
    //sevseg, am_pm and weekdays indicators
    byte digits[4]={0,0,0,0};
    bool isAM=true;
    byte weekdays = 0b11111110;

    //the following bytes will keep track
    //of index of active digit or weekday being edited,
    //of the alarm being edited or added
    //useful only in add,edit modes
    byte edit_activeDigit=0;
    byte edit_activeWeekday=0;

    //keeps track of the index of alarm being
    //edited or deleted. useless in add mode
    byte activeAlarmIndex=0;

    byte totalNoOfAlarms=0;  
    AlarmProvider& alarmProvider;
    SevSeg& sevSeg;
    SRLeds& indicator;
    SRLeds& weekdaysLeds;

    ButtonsManager& buttonsManager;

    //UTILITY
    bool isValid(byte digits[4]);
    int aggregateDigits(bool firstTwo, bool lastTwo);
    Alarm alarmFromVars();
    void loadAlarmToDispVars();
    void loadNextAlarmToDispVars();
    void loadPrevAlarmToDispVars();

    //RENDER
    void renderDisplay();
    void renderEditWeekdays();

    //INIT MODE
    void initAddMode();
    void initEditMode();
    void initDelMode();

    //INIT SUBMODE
    void initSubModeDigits();
    void initSubModeAmPm();
    void initSubModeWeekdays();

    //SWITCH SubMode
    void nextSubMode();
    void initCurrentSubMode();
    
    //END MODE
    void endAddMode(bool save);
    void endEditMode(bool save);
    void endDelMode(bool save);
  
  public:
    AlarmsUI(AlarmProvider& alarmProvider, SevSeg& sevSeg,
     SRLeds& indicator,SRLeds& weekdaysLeds,ButtonsManager& buttonsManager);
    void initialize();
    void process();
    void saveData();

};
