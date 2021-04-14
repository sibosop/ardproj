#ifndef BUG_H
#define BUG_H
#include "Adafruit_NeoPixel.h"


#include "RGB.h"

//#define DEBUG

#ifndef NUM_BUGS
#define NUM_BUGS  5
#endif

#include "BugRing.h"
class BugManager;

struct BugPos {
  int pos;
  int ring;
  bool off;
#ifdef DEBUG
  char debugBuf[15];
#endif
  BugPos() 
    : pos(0)
    , ring(0)
    , off(true)
  {}
  BugPos(int p, int r, bool o=false)
    : pos(p)
    , ring(r)
    , off(o)
  {}
#ifdef DEBUG
  const char *str() {
    sprintf(debugBuf,"r%d p%d %s",ring,pos,off?"off":"on");
    return debugBuf;
  }
#endif
  bool operator==(const BugPos& p) const {
    return ring == p.ring && pos == p.pos && off == p.off;
  }
  bool isCenter() const {
    return pos==0 && ring ==0 && !off;
  }
  void setCenter() {
    pos = 0;
    ring = 0;
  }
  void setOff() {
    off = true;
  }
  bool isOff() {
    return off;
  }
};

class Bug {
  friend BugManager;
private:
  int speed;
  int speedCount;
  RGB color;
  static const int MaxSpeed = 2000;
  static const int MinSpeed = 50;
  
public:
  BugPos pos;
  Bug(){};
  void reset()
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
      bugs[i].reset();
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
      if (bugs[i].pos.isOff()) {
        if (posOccupied(BugPos(0,0))) {
#ifdef DEBUG
          sprintf(buf,"bug[%d]: center occupied: %s\r\n",i,bugs[i].pos.str());
          Serial.print(buf);
#endif
          continue;
        }
#ifdef DEBUG
        sprintf(buf,"bug[%d]: initial: %s\r\n",i,bugs[i].pos.str());
        Serial.print(buf);
#endif        
        bugs[i].pos.off = false;
        bugs[i].reset();
        bugs[i].pos.setCenter();
        rval = true;
      }
      BugPos npos = bugRingManager.npos(bugs[i].pos);
      if (npos.isOff()) {
        bugs[i].pos = npos;
#ifdef DEBUG
        sprintf(buf,"bug[%d]: %s\r\n",i,bugs[i].pos.str());
        Serial.print(buf);
#endif
        continue;
      }
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
      if (bugs[i].pos.isOff()) continue;
      strip.setPixelColor(bugRingManager.getRealPos(bugs[i].pos), bugs[i].color.r, bugs[i].color.g, bugs[i].color.b);
    }
  }
};

extern BugManager bugManager;

#endif