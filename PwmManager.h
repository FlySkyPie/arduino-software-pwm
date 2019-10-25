#pragma once
#include <stdint.h>
#include "Scheduler.h"
#include "IDater.h"
#include "IPwmManager.h"
#include "IHardware.h"

class PwmManager : public IDater, public IPwmManager {
public:
  PwmManager(IHardware *hardware);
  uint32_t doDate();
  void setPower(uint8_t pinId, uint8_t power);
private:
  Scheduler *scheduler;
  IHardware *hardware;

  void doJobAtStep0(uint32_t &delay);
  void doJobAtStep(uint32_t &delay);
  void doJobAtStepEnd(uint32_t &delay);
  
  void setupPins();
  void turnOnScheduledPins();
  void turnOnFullWorkPins();
  void turnOffNonworkPins();

  void setDigital(uint8_t id, bool status);

  uint32_t period;
  uint32_t timeSpend;

  uint8_t currentStep;
  uint8_t scheduleCount;
  uint8_t *pinSchedule;
  uint8_t *powerSchedule;
};
