#pragma once

#define PIN_DISPLAY_DATA 2
#define PIN_DISPLAY_CLOCK 3

#define PIN_DISPLAY_COMM_1 4
#define PIN_DISPLAY_COMM_2 5
#define PIN_DISPLAY_COMM_3 6
#define PIN_DISPLAY_COMM_4 7

#define PIN_WEEKS_DISPLAY_DATA 2
#define PIN_WEEKS_DISPLAY_CLOCK 8

//TODO: Since Clock is held low while data is not written,
// it might be possible for these to have common data line.
// experiment and findout!
#define PIN_EXTRA_DISPLAY_DATA 2
#define PIN_EXTRA_DISPLAY_CLOCK 9

#define PIN_BTNS1_DATA 10
#define PIN_BTNS1_CLOCK 11
#define PIN_BTNS1_INPUT 12

//this led shows will the alarm ring or not
//had to assign a seperate pin, coz shift register is house full
#define PIN_LED_ALARMON 13

#define BTN_UP 0
#define BTN_DOWN 1
#define BTN_SET 2
#define BTN_ALARMS 3
#define BTN_DEL_AL 4
#define BTN_ADD_AL 5
#define BTN_BACK 6

//this button toggles whether alarm should ring or not
#define BTN_ALARMON 7


#define LED_TIMENOW 0
#define LED_DATE 1
#define LED_ALARM 2
#define LED_EDIT 3
#define LED_DEL 4
#define LED_ADD 5
#define LED_AM 6
#define LED_PM 7

