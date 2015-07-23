#pragma once

#include <Arduino.h>
#include "TimeProvider.h"
#include "ButtonsManager.h"
#include "SevSeg.h"
#include "SRLeds.h"
#include "UI.h"

class TimeDateSetUI : public UI {
  private:
    TimeProvider& timeProvider;
    ButtonsManager& buttonsManager;
    enum EDIT {
      EDIT_TIME,EDIT_AM_PM,EDIT_DATE,EDIT_YEAR
    };

    EDIT currentEdit;
    SevSeg& sevSeg;

    SRLeds& indicators;

    byte activeDigit= 1;
    byte digits[4]={0,0,0,0};

    bool isAM;
    int aggregateDigits(bool firstTwo,bool lastTwo);
    bool isValid(byte newDigits[4]) ;
    void initEditTime();
    void initEditAM_PM();
    void initEditDate();
    void initEditYear();
    
    void saveTime();
    void saveAM_PM();
    void saveDate();
    void saveYear();
    
    void initCurrentMode(); 
    void saveCurrentMode();
    void nextMode();
  public:
    TimeDateSetUI(TimeProvider& timeProvider,
		  ButtonsManager& buttonsManager,
		  SevSeg& sevSeg,
		  SRLeds& indicator);
    void initialize();
    void process();
    void saveData();
};
