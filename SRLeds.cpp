#include "SRLeds.h"
  SRLeds::SRLeds(byte srClockPin, byte srDataPin) {
    this->srClockPin = srClockPin;
    this->srDataPin = srDataPin;
  }
  void SRLeds::process() {
    if (updateDigits) {
        shiftOut(srDataPin, srClockPin, data, LSBFIRST);
	return;
    }
    if (blink) { //if any of the blink flag is set
      timeSinceLastBlink=millis() - lastBlink;
      if (timeSinceLastBlink > blinkDelay) { //the time we last blinked is greater than specified delay
	//by AND-ing data and (NOT blink) we get a new data that will only turn on
	//whose blink flag=0
	shiftOut(srDataPin, srClockPin, lastBlinkState ? data & (!blink) : data, LSBFIRST);
	lastBlink = millis();
	lastBlinkState = !lastBlinkState;

	//Note: timeSinceLastFastBlink = millis() - lastFastBlink
	//                             = millis() - (lastBlink+lastFastBlinkDelta)
	//                             = (millis() - lastBlink) - lastFastBlinkDelta
	//                             = timeSinceLastBlink - lastFastBlinkDelta
      } else if (timeSinceLastBlink-lastFastBlinkDelta > blinkDelayFast) {
	//by AND-ing data and (NOT (fastblink AND blink) ) we get a new data that will only turn off
	//whose (blink&fastblink) flag=1
	shiftOut(srDataPin, srClockPin, lastBlinkState ? data & (! (blink&&blinkFast) ) : data, LSBFIRST);
	lastFastBlinkDelta = millis()-lastBlink;
	lastFastBlinkState = !lastBlinkState;
        
     }
    }
  }
  
  void SRLeds::setLedOnOff(byte index, bool onOff) {
    byte mask = 0b10000000 >> index; //eg: index=3 -> mask=0b00100000;
    //eg: data = 0b10010110
    //then data|mask = 0b10010110 | 0b00100000 = 0b10110110
    // (!mask) = 0b11011111; data&(!mask) = 0b10010110 & 0b11011111 = 0b10010110
    data = (onOff) ? data | mask : data & (!mask);
    //if LED is set to be off then reset blink and blink fast flag
    blink = (onOff)?blink: blink & (!mask);
    blinkFast = (onOff)?blinkFast: blinkFast & (!mask);
    updateDigits = true;
  }
  void SRLeds::setAllLeds(byte datanew) {
    data = datanew;
    //resets blink and blinkFast flags for any led that is off
    blink = blink&data;
    blinkFast = blinkFast&blink;
    updateDigits = true;
  }

  void SRLeds::setLedBlink(byte index, bool blinkOnOff) {
    byte mask = 0b10000000 >> index; //eg: index=3 -> mask=0b00100000;
    //eg: this.blink = 0b10010110
    //then this.blink|mask = 0b10010110 | 0b00100000 = 0b10110110
    // (!mask) = 0b11011111; blink&(!mask) = 0b10010110 & 0b11011111 = 0b10010110
    blink = (blinkOnOff) ? blink | mask : blink & (!mask);
    //if blink is set to be off, reset blink fast flag
    blinkFast = (blinkOnOff)?blinkFast: blinkFast & (!mask);
  }
  void SRLeds::setAllLedsBlink(byte blinkData) {
    blink = blinkData;
    blinkFast = blinkFast&blinkData; //sets blinkFast flag 0, if blink flag == 0
  }
  void SRLeds::setLedBlinkFast(byte index, bool blinkFastOnOff) {
    byte mask = 0b10000000 >> index; //eg: index=3 -> mask=0b00100000;
    if(blink&mask==0) return; //if blink flag is not set exit 
    //eg: this.blinkFast = 0b10010110
    //then this.blinkFast|mask = 0b10010110 | 0b00100000 = 0b10110110
    // (!mask) = 0b11011111; blinkFast&(!mask) = 0b10010110 & 0b11011111 = 0b10010110
    blinkFast = (blinkFastOnOff) ? blinkFast | mask : blinkFast & (!mask);
  }
  void SRLeds::setAllLedsBlinkFast(byte blinkData) {
    blinkFast = blinkData&blink; //sets blinkFast flag 0, if blink flag == 0
  }
  
