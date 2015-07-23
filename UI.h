#pragma once

#include <Arduino.h>

class UI {
  public:
    virtual void initialize();
    virtual void process();
    virtual void saveData();
};
