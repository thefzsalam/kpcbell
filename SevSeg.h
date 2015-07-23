#include <Arduino.h>
#pragma once

/*
 *  f_
 * e|_|a
 * d|_|b
 *   c
 */
//SevSeg(srClock,srData);
class SevSeg{
  private:
     const byte digits[11] = {
       0b11111100,0b11000000,0b10110110,0b11100110,0b11001010,0b01101110,0b01111110,0b11000100,0b11111110,0b11101110,0b00000000
     };
     byte srClockPin;
     byte srDataPin;
     byte commonPins[4];

     bool blink[4] = {false,false,false,false};

     //these flags are set by blink processing section
     //it is set to on and off according to the delay
     bool blinkDigitOn[4] = {true,true,true,true};

     byte digitsDisplay[4]={8,8,8,8};
     bool commonAnode;
     long lastMultiplexed=0;
     byte multiplexIndex = 0;
     const byte multiplexDelay = 10;//ms

     const int blinkDelay=500; //ms
     long lastBlink=0;
     bool lastBlinkState=true;
  public:
     SevSeg(byte srClockPin, byte srDataPin,byte commonPins[4],bool commonAnode);
     void process();
     void setAllDigits(byte hour10,byte hour1,byte min10,byte min1);
     void setNthDigit(byte n,byte val);
     void setBlink(byte n,bool blink);
};

