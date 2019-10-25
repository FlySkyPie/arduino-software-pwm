#pragma once
#include <stdint.h>

class IHardware {
public:
  virtual void setDigital(uint8_t pin, bool value) = 0;
};