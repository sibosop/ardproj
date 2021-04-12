#include "BugRing.h"
#include "Bug.h"

#ifndef RING_COUNT
#define RING_COUNT 6
#endif

const BugRingManager::BugRing BugRingManager::bugRings[MAX_RINGS] =
{
  BugRing(1,0),
  BugRing(8,1),
  BugRing(12,9),
  BugRing(16,21),
  BugRing(24,37),
  BugRing(32,61),
  BugRing(40,93),
  BugRing(48,133),
  BugRing(60,181)
};

BugRingManager bugRingManager;

int BugRingManager::getRealPos(const BugPos& p) const {
  return p.pos + bugRings[p.ring].offset;
}

BugPos BugRingManager::npos(const BugPos& p) {
  BugPos rval(p);
  rval.ring = (rval.ring-1) + random(0,3);
  
  // edge conditions where bug falls off
  if ( (rval.ring < 0) ||  (rval.ring == RING_COUNT))
  {
    rval.setNoRing();
    return rval;
  }
  // Center condition
  if (!rval.ring) {
    rval.setCenter();
    return rval;
  }
  
  // moving from 0 to first ring is also special condition
  if ((rval.ring == 1) && p.isCenter())
  {  
    rval.pos = random(0,8);
    return rval;
  }
  
  float scnt = bugRings[p.ring].cnt;
  float ncnt = bugRings[rval.ring].cnt;
  float rat = scnt / ncnt;
  float np = (float) p.pos * rat;
  //printf("r = %f np = %f\n", r, np);
  if ( floor(np) == ceil(np) ) {
    switch (random(0,3))
    {
      case 0:
        //printf("doing sub ");
        if ( floor(np) == 0)
          rval.pos = ncnt-1;
        else
          rval.pos = floor(np) - 1;
        break;

      case 1:
        //printf("doing none ");
        rval.pos = (int) np;
        break;
      
      case 2:
        //printf("doing plus ");
        if ( floor(np)+1 == (ncnt))
          rval.pos =  0;
        else
          rval.pos =  floor(np) + 1;
      default:
        break;
    }
  } else {
    if ( random(0,2) )  {
      //printf("doing floor ");
      rval.pos = floor(np);
    }
    else
    {
      //printf("doing ceil ");
      rval.pos = ceil(np);
    }
  }
  return rval;
}