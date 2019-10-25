#pragma once
#include <stdint.h>

class IPwmManager {
public:
  virtual void setPower(uint8_t pinId, uint8_t power) = 0;
};