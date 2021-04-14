#ifndef BUG_H
#define BUG_H
#include "Adafruit_NeoPixel.h"


#include "RGB.h"

#define DEBUG

#ifndef NUM_BUGS
#define NUM_BUGS  5
#endif

#include "BugRing.h"
class BugManager;

struct BugPos {
  int pos;
  int ring;
#ifdef DEBUG
  char debugBuf[15];
#endif
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
  
#ifdef DEBUG
  const char *str() {
    sprintf(debugBuf,"r%d p%d",ring,pos);
    return debugBuf;
  }
#endif
  
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
  friend BugManager;
private:
  int speed;
  int speedCount;
  RGB color;
  static const int MaxSpeed = 1000;
  static const int MinSpeed = 500;
  
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
#ifdef DEBUG
      char buf[50];
#endif
      
      // are we in off the board position
      if (!bugs[i].pos) {
        if (posOccupied(BugPos(0,0))) {
#ifdef DEBUG
          sprintf(buf,"bug[%d]: center occupied: %s\r\n",i,bugs[i].pos.str());
          Serial.print(buf);
#endif
          return false;
        }
#ifdef DEBUG
        sprintf(buf,"bug[%d]: initial: %s\r\n",i,bugs[i].pos.str());
        Serial.print(buf);
#endif        
        bugs[i].init();
        bugs[i].pos.setCenter();
        rval = true;
      }
      BugPos npos = bugRingManager.npos(bugs[i].pos);
      if (posOccupied(npos))
        continue;
      bugs[i].pos = npos;
      rval = true;
      
#ifdef DEBUG
      sprintf(buf,"bug[%d]: %s\r\n",i,bugs[i].pos.str());
      Serial.print(buf);
#endif
    }
    
    return rval;
  }
  void setStrip(Adafruit_NeoPixel& strip) {
    for (int i = 0; i < NUM_BUGS; ++i ) {
      if (!bugs[i].pos) continue;
      strip.setPixelColor(bugRingManager.getRealPos(bugs[i].pos), bugs[i].color.r, bugs[i].color.g, bugs[i].color.b);
    }
  }
};

extern BugManager bugManager;

#endif