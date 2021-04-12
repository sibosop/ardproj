
#ifndef BUG_RING
#define BUG_RING
#include <math.h>       /* round, floor, ceil, trunc */

#define MAX_RINGS 9

class BugPos;
class BugRingManager {
  struct BugRing {
    int cnt;
    int offset;
    BugRing(int c,int o)
      : cnt(c) 
      , offset(o)
    {}
  };
  static const BugRing bugRings[MAX_RINGS];
public:
  BugPos npos(const BugPos& p);
};

extern BugRingManager bugRingManager;


#endif