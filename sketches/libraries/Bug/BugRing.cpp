#include "BugRing.h"
#include "Bug.h"

#ifndef RING_COUNT
#define RING_COUNT 6
#endif
//#define DEBUG_RING

const BugRingManager::BugRing BugRingManager::bugRings[MAX_RINGS] =
{
  BugRing(1,0), //0
  BugRing(8,1), //1
  BugRing(12,9),//2
  BugRing(16,21),//3
  BugRing(24,37),//4
  BugRing(32,61),//5
  BugRing(40,93),//6
  BugRing(48,133),//7
  BugRing(60,181)//8
};

BugRingManager bugRingManager;

int BugRingManager::getRealPos(const BugPos& p) const {
  return p.pos + bugRings[p.ring].offset;
}

BugPos BugRingManager::npos(BugPos& p) {
  BugPos rval(p);
#ifdef DEBUG_RING
  char db[200];
#endif
#ifdef DEBUG_RING
  sprintf(db,"rval = %s p = %s",rval.str(),p.str());
  Serial.println(db);
#endif
  if (!rval.isCenter())
    rval.ring = (rval.ring-1) + random(0,3);
  else
    rval.ring = random(0,2);
  
#ifdef DEBUG_RING
  sprintf(db,"new ring %d",rval.ring);
  Serial.println(db);
#endif
  
  // edge conditions where bug falls off
  if ( rval.ring == RING_COUNT )
  {
#ifdef DEBUG_RING
    sprintf(db,"bug off back to center with new color");
    Serial.println(db);
#endif
    rval.setCenter();
    rval.setOff();
    return rval;
  }
  // Center condition
  if (!rval.ring) {
#ifdef DEBUG_RING
  sprintf(db,"setting center");
  Serial.println(db);
#endif
    rval.setCenter();
    return rval;
  }
  
  // moving from 0 to first ring is also special condition
  if ((rval.ring == 1) && p.isCenter())
  {  
    rval.pos = random(0,8);
#ifdef DEBUG_RING
  sprintf(db,"setting from center %s",rval.str());
  Serial.println(db);
#endif
    return rval;
  }
  float scnt = bugRings[p.ring].cnt;
  float ncnt = bugRings[rval.ring].cnt;
  float rat = ncnt / scnt;
  
#ifdef DEBUG_RING
  sprintf(db,"from ring %d to ring %d rat %s pos %d"
    , p.ring
    , rval.ring
    , String(rat).c_str()
    , p.pos
    );   
  Serial.println(db);
#endif
  
  if ( p.ring == rval.ring ) {
    if ( random(0,2) ) {
      rval.pos += 1;
      if ( rval.pos == bugRings[p.ring].cnt )
        rval.pos = 0;
    } else {
      if (!rval.pos) 
        rval.pos = bugRings[p.ring].cnt - 1;
      else
        rval.pos -= 1;
    }
#ifdef DEBUG_RING
    sprintf(db,"same ring %d moved from %d to %d", rval.ring, p.pos, rval.pos);
    Serial.println(db);
#endif
    return rval;
  }
  
  float np = (float) p.pos * rat;
  float fl = floor(np);
  float ce = ceil(np);
#ifdef DEBUG_RING
  sprintf(db,"new pos %s floor %s ceil %s", String(np).c_str(), String(fl).c_str(), String(ce).c_str() );
  Serial.println(db);
#endif
  
  if ( fl == ce ) {
    switch (random(0,3))
    {
      case 0:
        //printf("doing sub ");
        if ( fl == 0)
          rval.pos = ncnt-1;
        else
          rval.pos = fl - 1;
        break;

      case 1:
        //printf("doing none ");
        rval.pos = (int) np;
        break;
      
      case 2:
        //printf("doing plus ");
        if ( fl+1 == (ncnt))
          rval.pos =  0;
        else
          rval.pos =  fl + 1;
      default:
        break;
    }
  } else {
    if ( random(0,2) )  {
      //printf("doing floor ");
      rval.pos = fl;
    }
    else
    {
      //printf("doing ceil ");
      rval.pos = ce;
    }
  } 
  return rval;
}