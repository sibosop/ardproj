#include "LedMatrix.h"
#include "trap.h"

const Pos Trap::positions[4] =
{
  Pos(3,3)
  ,Pos(4,4)
  ,Pos(5,3)
  ,Pos(4,2)
};

const Color Trap::colors[4] =
{
  Color(0x10,0)
  ,Color(0x1f,0x1f)
  ,Color(0x0,0x1f)
  ,Color(0x10,0x1f)
};

void
Trap::display()
{
  if ( !(count++ % 20) )
    ++swap;
  
  for (int i = 0; i < 4; ++i)
  {
    Pixel p;
    p.p = positions[i];
    p.c = colors[(i+swap) % 4];
    LedMatrix.setPixel(p);
  }
}