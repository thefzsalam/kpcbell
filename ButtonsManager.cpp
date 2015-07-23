#include <Arduino.h>
#include "ButtonsManager.h"

ButtonsManager::ButtonsManager(SRButtons& srButtons, TimeProvider& tp):srButtons(srButtons), tp(tp) {
}

bool ButtonsManager::isSpAndRel(byte button_const) {
  return buttons[button_const].isSpRel?!(buttons[button_const].isSpRel = false):false;
}
bool ButtonsManager::isLpAndRel(byte button_const) {
  return buttons[button_const].isLpRel?!(buttons[button_const].isLpRel = false):false;
}

void ButtonsManager::process() {
  for(byte i=0;i<8;i++) {
    bool pressed = srButtons.isPressed(i);
    if(pressed && !buttons[i].isPressed) {
      buttons[i].isPressed = true;
      buttons[i].pressedSince = tp.millis();
    } else if(!pressed && srButtons.isPressed(i)) {
      buttons[i].isPressed = false;
      buttons[i].pressedSince = 0;
      if(tp.millis()-buttons[i].pressedSince > DELAY_LONG_PRESS) {
	buttons[i].isSpRel = !(buttons[i].isLpRel=true);
      } else {
	buttons[i].isSpRel = !(buttons[i].isLpRel=false);
      }
    }
  }
}
