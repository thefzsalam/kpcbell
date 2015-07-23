#pragma once

class Button {
  private:
    byte pin;
    byte mode;
  public:
    Button(byte pin, byte mode) {
      this->pin = pin;
      this->mode = mode;
      pinMode(pin, INPUT);
    }
    bool isPressed() {
      return (digitalRead(pin) == mode);
    }
};

