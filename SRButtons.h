#pragma once

#include<Arduino.h>
class SRButtons{
  private:
    byte srClockPin;
    byte srDataPin;
    byte btnInputPin;
    
    static const byte dbncDelay = 20; //ms
    bool isPressedNow=false;
    long pressedSince=0;//ms


    bool highIsOn;

  public:
    SRButtons(byte srClockPin, byte srDataPin, byte btnInputPin,bool highIsOn); 
    bool isPressed(byte btn);
};

