#include <Arduino.h>
#include "SevSeg.h"
/*
 *  f_
 * e|_|a
 * d|_|b
 *   c
 */

//SevSeg(srClock,srData);
SevSeg::SevSeg(byte srClockPin, byte srDataPin,byte commonPins[4], bool commonAnode) {
  this->srClockPin = srClockPin;
  this->srDataPin = srDataPin;
  for(byte b=0;b<4;b++) {
    this->commonPins[b] = commonPins[b];
  }
  this->commonAnode = commonAnode;
}


void SevSeg::process() {

  if(lastMultiplexed-millis() > multiplexDelay) {
    //if blink[multiplexIndex] is false, the below expression always gives true
    if(blinkDigitOn[multiplexIndex]||!blink[multiplexIndex]) {
      //turnOff previous digits while SR is changing to avoid ghosting
      digitalWrite(commonPins[multiplexIndex],commonAnode?LOW:HIGH);
      //to draw next digit, increment multiplexIndex
      multiplexIndex=multiplexIndex<3?multiplexIndex+1:0;
      //shift out new digit's data
      shiftOut(srDataPin,srClockPin,LSBFIRST,digits[digitsDisplay[multiplexIndex]]);
      //turn on new digit
      digitalWrite(commonPins[multiplexIndex],commonAnode?HIGH:LOW);
    } 
    //even if digit is not turned on,incrment the multiplexIndex
    else multiplexIndex=multiplexIndex<3?multiplexIndex+1:0;
   
    lastMultiplexed=millis();
  }
  
  if(blink[0]||blink[1]||blink[2]||blink[3]) { //if any of the blink flag is set
    if(millis()-lastBlink>blinkDelay) {//time since blinked is gr8r than blink delay
      //if(!blink[i]) OR'ing it with anything will always return true;
      blinkDigitOn[0]=lastBlinkState||(!blink[0]);
      blinkDigitOn[1]=lastBlinkState||(!blink[1]);
      blinkDigitOn[2]=lastBlinkState||(!blink[2]);
      blinkDigitOn[3]=lastBlinkState||(!blink[3]);
      lastBlink = millis();
      lastBlinkState=!lastBlinkState;
    }
  }
}

void SevSeg::setAllDigits(byte hour10,byte hour1,byte min10,byte min1) {
  digitsDisplay[0]=hour10; digitsDisplay[1]=hour1; digitsDisplay[2]=min10;digitsDisplay[3]=min1;
}
void SevSeg::setNthDigit(byte n,byte val) { //CAUTION: n=0,1,2 or 3 only
  digitsDisplay[n]=val;
}
void SevSeg::setBlink(byte n,bool blink) {
  this->blink[n]=blink;
}
