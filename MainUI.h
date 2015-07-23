#pragma once

#include <Arduino.h>
#include "Constants.h"

#include "UI.h"

#include "AlarmProvider.h"
#include "TimeProvider.h"

#include "SevSeg.h"
#include "SRLeds.h"

class MainUI: public UI {
 private:
    enum DISPLAY_MODE {
     DISPLAY_TIME_NOW,DISPLAY_DATE,DISPLAY_NEXT_ALARM
    };

    DISPLAY_MODE currentDisplay;
    long lastUpdated;
    SevSeg& sevSeg;
    SRLeds& weeksLed;
    
    SRLeds& indicators;
    TimeProvider& timeProvider;
    
    AlarmProvider& alarmProvider;
    
    const int delayUpdate = 2000;

    void turnOnIndicators(bool time,bool data,bool alarm,bool am);
  public:
    MainUI(SevSeg& sevSeg,SRLeds& weeksLed, SRLeds& indicator,TimeProvider& timeProvider,AlarmProvider& alarmProvider); 
    void process();
    void saveData();
    void initialize();
};
