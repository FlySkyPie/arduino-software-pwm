#include "ArduinoHardware.h"

ArduinoHardware::ArduinoHardware() {
}

void ArduinoHardware::setDigital(uint8_t pin, bool value) {
  if (value) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}
