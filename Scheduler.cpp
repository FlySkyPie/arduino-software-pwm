/*
 * This class is used for software PWM.
 */

#include "Scheduler.h"

Scheduler::Scheduler(uint8_t size) {
  this->initialize(size);
}

Scheduler::~Scheduler() {
  delete [] this->exception;
  delete [] this->power;
  delete [] this->pinSchedule;
  delete [] this->powerSchedule;
}

/*
 * Set new mission.
 */
void Scheduler::setMission(uint8_t id, uint8_t power) {
  if (id>this->size) {
    return;
  }
  if (power == 0 || power == 0xff) {
    this->exception[id] = true;
  } else {
    this->exception[id] = false;
  }
  this->power[id] = power;
  this->changed = true;
}

void Scheduler::updateSchedule() {
  if (!this->changed) {
    return;
  }

  this->createSchedule();
  this->loadSchedule();
  this->sortSchedule();

  this->reloadFullPins();
  this->reloadOffPins();

  this->changed = false;

}

/*
 * Get schedule table.
 */
void Scheduler::getSchedule(uint8_t **pins, uint8_t **powers, uint8_t &size) {
  *pins = this->pinSchedule;
  *powers = this->powerSchedule;
  size = this->scheduledCount;

}

/*
 * Get tablle of pins which turn full on.
 */
void Scheduler::getFullPins(uint8_t **pins, uint8_t &size) {
  *pins = this->fullPins;
  size = this->fullPinsCount;
}

/*
 * Get tablle of pins which turn full off.
 */
void Scheduler::getOffPins(uint8_t **pins, uint8_t &size) {
  *pins = this->offPins;
  size = this->offPinsCount;
}

/*
 * reload pins which turn full on.
 */
void Scheduler::reloadFullPins() {
  delete [] this->fullPins;
  this->fullPins = new uint8_t[this->getFullPinsCount()];

  uint8_t i = 0, j = 0;
  while (i < this->size) {
    if (this->power[i] == 255) {
      this->fullPins[j] = i;
      j++;
    }
    i++;
  }
}

/*
 * reload pins which turn off.
 */
void Scheduler::reloadOffPins() {
  delete [] this->offPins;
  this->offPins = new uint8_t[this->getOffPinsCount()];

  uint8_t i = 0, j = 0;
  while (i < this->size) {
    if (this->power[i] == 0) {
      this->offPins[j] = i;
      j++;
    }
    i++;
  }
}

/*
 * Recount number of full pins and return it.
 */
uint8_t Scheduler::getFullPinsCount() {
  this->fullPinsCount = 0;
  for (uint8_t i = 0; i<this->size; i++) {
    if (this->power[i] == 255) {
      this->fullPinsCount += 1;
    }
  }
  return this->fullPinsCount;
}

/*
 * Recount number of off pins and return it.
 */
uint8_t Scheduler::getOffPinsCount() {
  this->offPinsCount = 0;
  for (uint8_t i = 0; i<this->size; i++) {
    if (this->power[i] == 0) {
      this->offPinsCount += 1;
    }
  }
  return this->offPinsCount;
}

/*
 * Initialize object, create dynamic array,
 * fill the status array.
 */
void Scheduler::initialize(uint8_t size) {
  this->changed = true;
  this->size = size;

  this->exception = new bool[size];
  this->power = new uint8_t[size];

  //fill array
  for (uint8_t i = 0; i<this->size; i++) {
    this->exception[i] = true;
    this->power[i] = 0;
  }

  this->pinSchedule = new uint8_t[0];
  this->powerSchedule = new uint8_t[0];
  this->offPins = new uint8_t[0];
  this->fullPins = new uint8_t[0];
}

/*
 * Release old schedule table, and create new dynamic array as schedule table.
 */
void Scheduler::createSchedule() {
  delete [] this->pinSchedule;
  delete [] this->powerSchedule;

  this->scheduledCount = 0;
  for (uint8_t i = 0; i<this->size; i++) {
    if (!this->exception[i]) {
      this->scheduledCount += 1;
    }
  }

  this->pinSchedule = new uint8_t[this->scheduledCount];
  this->powerSchedule = new uint8_t[this->scheduledCount ];
}

/*
 * Load pin id and power to schedule table from status table.
 * Ignore exception pin.
 */
void Scheduler::loadSchedule() {
  uint8_t i = 0, j = 0;
  while (i < this->size) {
    if (!this->exception[i]) {
      this->pinSchedule[j] = i;
      this->powerSchedule[j] = this->power[i];
      j++;
    }
    i++;
  }
}

/*
 * Sort schedule by power value.
 */
void Scheduler::sortSchedule() {
  for (uint8_t i = 0; i < this->scheduledCount - 1; i++) {
    for (uint8_t j = 0; j < this->scheduledCount - i - 1; j++) {
      if (this->powerSchedule[j] <= this->powerSchedule[j + 1]) {
        continue;
      }
      uint8_t tempId = pinSchedule[j + 1];
      uint8_t tempPower = powerSchedule[j + 1];

      this->pinSchedule[j + 1] = this->pinSchedule[j];
      this->powerSchedule[j + 1] = this->powerSchedule[j];

      this->pinSchedule[j] = tempId;
      this->powerSchedule[j] = tempPower;
    }
  }
}
