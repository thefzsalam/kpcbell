#pragma once

#include<Arduino.h>
class SRLeds {
  private:
    byte data = 0b000000000;
    byte srClockPin;
    byte srDataPin;
    bool updateDigits = false;
    byte blink = 0b00000000;
    byte blinkFast = 0b00000000;
    
    const int blinkDelay = 500; //ms
    const byte blinkDelayFast = 250; //ms

    long lastBlink = 0;
    int lastFastBlinkDelta = 0; //obtain lastBlinkFast by adding this to lastBlink
    int timeSinceLastBlink = 0;
    bool lastBlinkState = true;
    bool lastFastBlinkState = true;

  public:
    SRLeds(byte srClockPin, byte srDataPin); 
    void process(); 
    void setLedOnOff(byte index, bool onOff); 
    void setAllLeds(byte datanew); 

    void setLedBlink(byte index, bool blinkOnOff); 
    void setAllLedsBlink(byte blinkData); 

    void setLedBlinkFast(byte index, bool blinkFast); 
    void setAllLedsBlinkFast(byte blinkFast); 
};

