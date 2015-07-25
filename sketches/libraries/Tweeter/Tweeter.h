#ifndef TWEETER_H
#define TWEETER_H
#include <arduino.h>
#include "SoftTimer.h"
#include "WavTrigger.h"

#define WAIT_INTERVAL 5  
class Tweeter : public Task 
{
public:
  Tweeter();
  void begin();
  void setList(const char *);
  bool needsList();
  
private:
  void nextTrack();
  void setNewTrack();
  void check();
  static void msgCallback(Task *t);
  
  WavTrigger wavTrigger;
  int msgCount;
  WTInfo info;
  WTTrackList trackList;
  int lastnum;
  bool sendTrack;
  uint16_t waitTimeout;
  uint16_t track;
  int repCount;
  
  char list[40];
  char newList[40];
  char *lp;
  
};
#endif