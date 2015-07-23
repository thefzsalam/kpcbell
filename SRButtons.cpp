#include "SRButtons.h"
#include <Arduino.h>

  SRButtons::SRButtons(byte srClockPin, byte srDataPin, byte btnInputPin,bool highIsOn) {
    this->srClockPin = srClockPin;
    this->srDataPin = srDataPin;
    this->btnInputPin = btnInputPin;
    this->highIsOn = highIsOn;
  }
    //IMPORTANT: Attach pulldown or pullup resistor to btnInputPin
    //according to high is on
  bool SRButtons::isPressed(byte btn) {
    byte mask=0b00000001 << btn;
    //TODO: possible bug here. experiment for highIsOn=false
    shiftOut(srDataPin, srClockPin, LSBFIRST,highIsOn?mask:byte(~mask));
    bool currentState = (digitalRead(btnInputPin) == HIGH)==highIsOn; 
    if (currentState&&isPressedNow) {
      return (millis()-pressedSince>dbncDelay);
    } else if(currentState) {
      isPressedNow=true;
      pressedSince=millis();      
    } else {
      isPressedNow=false;
      pressedSince=0;
    }
    return false;
  }

