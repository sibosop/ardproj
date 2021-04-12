#ifndef BUG_H
#define BUG_H


#include "RGB.h"


#ifndef NUM_BUGS
#define NUM_BUGS  5
#endif

#include "BugRing.h"

struct BugPos {
  int pos;
  int ring;
  BugPos() 
    : pos(-1)
    ,ring(-1)
  {}
  BugPos(int p, int r)
    : pos(p)
    , ring(r)
  {}
  bool operator!() {
    return pos == -1 ;
  }
  bool operator==(const BugPos& p) const {
    return ring == p.ring && pos == p.pos;
  }
  void setNoRing() {
    pos = -1;
    ring = -1;
  }
  bool isCenter() const {
    return pos==0 && ring ==0;
  }
  void setCenter() {
    pos = 0;
    ring = 0;
  }
};

class Bug {
private:
  
  int speed;
  int speedCount;
  RGB color;
  static const int MaxSpeed = 500;
  static const int MinSpeed = 100;
public:
  BugPos pos;
  Bug(){};
  void init()
  {
    color.reset();
    setSpeed();
  }
  
  
  bool ready() { 
    if ( ++speedCount < speed )
      return false;
    speedCount = 0;
    return true;
  }
  bool atPos(const BugPos& p) {
    if ( !pos.pos )
      return false;
    return p == pos;
  }
private:
  void setSpeed() {
    speed = random(MinSpeed,MaxSpeed);
    speedCount = 0;
  }
};

class BugManager {
private:
  Bug bugs[NUM_BUGS];
public:
  BugManager(){}
  void init() {
    for (int i = 0; i < NUM_BUGS; ++i) {
      bugs[i].init();
    }
  }
  bool posOccupied(const BugPos& pos) {
    for (int i = 0; i < NUM_BUGS; ++i )
    {
      if (bugs[i].atPos(pos)) {
        return true;
      } 
    }
    return false;
  }
  bool process() {
    bool rval = false;
    for (int i = 0; i < NUM_BUGS; ++i )
    {
      if (!bugs[i].ready())
        continue;
      // are we in off the board position
      if (!bugs[i].pos) {
        if (posOccupied(BugPos(0,0)))
          return false;
        bugs[i].init();
        bugs[i].pos.setCenter();
        rval = true;
      }
      BugPos npos = bugRingManager.npos(bugs[i].pos);
      if (posOccupied(npos))
        continue;
      bugs[i].pos = npos;
      rval = true;
    }
    return rval;
  }
};

extern BugManager bugManager;

#endif