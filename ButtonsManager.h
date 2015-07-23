#pragma once

#include<Arduino.h>
#include "SRButtons.h"
#include "TimeProvider.h"

class ButtonsManager {
  public:
    struct Button{
      bool isPressed = false;
      bool isSpRel = false;
      bool isLpRel = false;
      long pressedSince = 0;
    };

    Button buttons[8];

    SRButtons& srButtons;
    TimeProvider& tp;
    
    ButtonsManager(SRButtons& srButtons,TimeProvider& tp);
    bool isSpAndRel(byte button_const);
    bool isLpAndRel(byte button_const);
    void process();
    const int DELAY_LONG_PRESS = 1000;//ms
}; 
