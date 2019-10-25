#pragma once
#include <stdint.h>

class Scheduler {
public:
  Scheduler(uint8_t size);
  ~Scheduler();

  void setMission(uint8_t id, uint8_t power);
  void updateSchedule();
  void getSchedule(uint8_t **pins, uint8_t **power, uint8_t &size);
  void getFullPins(uint8_t **pins, uint8_t &size);
  void getOffPins(uint8_t **pins, uint8_t &size);
private:
  void initialize(uint8_t size);
  void createSchedule();
  void loadSchedule();
  void sortSchedule();

  void reloadFullPins();
  void reloadOffPins();

  uint8_t getFullPinsCount();
  uint8_t getOffPinsCount();

  /*
   * Is status table has changed?
   * There are new mission have been added?
   */
  bool changed;

  /*
   * Main status table.
   */
  uint8_t size;
  bool *exception;
  uint8_t *power;

  /*
   * Schedule table.
   */
  uint8_t scheduledCount;
  uint8_t *pinSchedule;
  uint8_t *powerSchedule;

  uint8_t *fullPins;
  uint8_t fullPinsCount;
  uint8_t *offPins;
  uint8_t offPinsCount;
};