#pragma once 
#include <Arduino.h>
#include <stdint.h>
#include "IHardware.h"

class ArduinoHardware : public IHardware {
public:
  ArduinoHardware();
  void setDigital(uint8_t pin, bool value);

};
