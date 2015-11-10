#include "LedMatrix.h"
#include "trap.h"

const Pos Trap::positions[] =
{
  Pos(3,3)
  ,Pos(4,4)
  ,Pos(5,3)
  ,Pos(4,2)
  ,Pos(3,4)
  ,Pos(5,4)
  ,Pos(5,2)
  ,Pos(3,2)
};

const Color Trap::colors[4] =
{
  Color(0x10,0)
  ,Color(0x0,0x1f)
  ,Color(0x1f,0x1f)
  ,Color(0x5,0x1f)
};

void
Trap::display()
{
  if ( !(count++ % 2) )
  {
    if ( !offset )
      offset = 4;
    else
    {
      swap += 3;
      offset = 0;
    }
  }
  
  for (int i = 0; i < 4; ++i)
  {
    Pixel p;
    p.p = positions[i+offset];
    p.c = colors[(i+swap)%4];
    LedMatrix.setPixel(p);
  }
}