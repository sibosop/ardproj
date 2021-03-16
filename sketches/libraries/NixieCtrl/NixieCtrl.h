#ifndef NIXIE_CTRL
#define NIXIE_CTRL
#include <SoftTimer.h>
#include <stdint.h>

class NixieCtrl : public Task
{
private:
	static const int numTubes = 4;
	static const int clk = 13;
	static const int mosi = 11;
  static const int displayStart = 2;
  static const int displayEnd = 65;
  static const int displaySetDelay = 5;
  static const double refreshTime = .08;
public:
  NixieCtrl(uint8_t latchPin_,uint8_t enablePin_);

  void set(uint8_t num, uint8_t val);


  void init();
	
private:
	uint8_t latchPin;
	uint8_t enablePin;
  uint8_t resetPin;
  static void refreshCallback(Task *);	
  void refresh();
  void clearShiftRegisters();
  void disable();
  void enable();
  int blankTimer;
  uint8_t tubeCnt;
  uint8_t tubeVal[4];
  uint8_t mask;
};

extern NixieCtrl NixieControl;

#endif