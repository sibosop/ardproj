#ifndef OMRON_H
#define OMRON_H
#include <SoftTimer.h>
#include <stdint.h>


class Omron : public Task
{
public:
  Omron(uint8_t,uint8_t);
  void begin();
  void set(uint8_t i);
  void go();
  bool ready(uint8_t);
  static const int numMotors=8;
private:
  static const double loopInterval=1;
  
  static const int offTimerCount=1;
  static const int runInterval=2500;
  static const int readyCount=runInterval/offTimerCount;
  static void handler(Task*);
  void handlerLoop();
  void clearShiftRegisters();
  void send(uint8_t);
  
  uint8_t motorTimer[numMotors];
  uint16_t readyTimer[numMotors];
  bool onList[numMotors];
  bool setList[numMotors];
  uint8_t latch;
  uint8_t clear;
};
#endif