#include "PwmManager.h"
//#include <iostream>

/*
 * Initial variables,
 * set first mission as 1 period time later.
 */
PwmManager::PwmManager(IHardware *hardware) {
  this->period = 10000; //30000-5,20000-10
  this->currentStep = 0;
  this->timeSpend = 0;
  this->hardware = hardware;

  this->scheduler = new Scheduler(6);
}

/*
 * Set PWM value of certain pin.
 */
void PwmManager::setPower(uint8_t pinId, uint8_t power) {
  if (pinId >= 6 || power >= 256) {
    return;
  }
  power = power /20 *20;
  if(power<20){
    power =0;
  }
  this->scheduler->setMission(pinId, power);
}

/* 
 * The tick function,
 * executed by timer 1.
 */
uint32_t PwmManager::doDate() {
  uint32_t delayTime;
  do {
    uint8_t currentStep = this->currentStep;

    if (currentStep == 0) {
      this->doJobAtStep0(delayTime);
    } else if (currentStep == this->scheduleCount) {
      this->doJobAtStepEnd(delayTime);
    } else {
      this->doJobAtStep(delayTime);
    }
  } while (delayTime  == 0);

  return delayTime;
}

void PwmManager::setupPins() {
  this->turnOnScheduledPins();
  this->turnOnFullWorkPins();
  this->turnOffNonworkPins();
}

/*
 * In step 0, turn on all pins which non ignored,
 * and update table of schedule.
 */
void PwmManager::doJobAtStep0(uint32_t &delay) {
  this->scheduler->updateSchedule();
  this->setupPins();

  //debug
  //for(uint8_t i=0;i<this->scheduleCount;i++){
  //  std::cout << (int)this->pinSchedule[i] << ",";
  //}
  //std::cout <<"\n";

  if (this->scheduleCount == 0) {
    delay = this->period;
    return;
  }

  delay = this->powerSchedule[0] * this->period / 255;
  this->timeSpend += delay;
  this->currentStep += 1;
}

/*
 * In normal step, turn scheduled pin off.
 */
void PwmManager::doJobAtStep(uint32_t &delay) {
  this->setDigital(this->pinSchedule[this->currentStep - 1], 0);
  uint32_t oldTimeSpend = this->timeSpend;
  this->timeSpend = this->powerSchedule[this->currentStep] * this->period / 255;
  delay = this->timeSpend - oldTimeSpend;
  this->currentStep += 1;
}

/*
 * In end step, turn scheduled pin off,
 * reset variables to make next step was step 0.
 */
void PwmManager::doJobAtStepEnd(uint32_t &delay) {
  this->setDigital(this->pinSchedule[this->currentStep - 1], 0);
  delay = this->period - this->timeSpend;
  this->timeSpend = 0;
  this->currentStep = 0;
}

void PwmManager::turnOnScheduledPins() {
  this->scheduler->getSchedule(
          &this->pinSchedule, &this->powerSchedule, this->scheduleCount);
  for (uint8_t i = 0; i<this->scheduleCount; i++) {
    uint8_t id = this->pinSchedule[i];
    this->setDigital(id, 1);
  }
}

void PwmManager::turnOnFullWorkPins() {
  uint8_t *pins, size;
  this->scheduler->getFullPins(&pins, size);
  for (uint8_t i = 0; i < size; i++) {
    this->setDigital(pins[i], 1);
  }
}

void PwmManager::turnOffNonworkPins() {
  uint8_t *pins, size;
  this->scheduler->getOffPins(&pins, size);
  for (uint8_t i = 0; i < size; i++) {
    this->setDigital(pins[i], 0);
  }
}

/*
 * Mapping pin's id to real pin number.
 * In this case, is 2~7.
 */
void PwmManager::setDigital(uint8_t id, bool status) {
  //debug
  //std::cout << "id: " << (int)id << ", " << status <<"\n";

  uint8_t base = 3;
  this->hardware->setDigital(base + id, status);
}
