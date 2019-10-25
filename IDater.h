#pragma once
#include <stdint.h>

class IDater {
public:
  virtual uint32_t doDate() = 0;
};