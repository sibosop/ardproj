#include "LedMatrix.h"
#include "Fly.h"

uint8_t Fly::idCount = 0;

const Pos Fly::corners[4] =
{
  Pos(0,0)
  ,Pos(0,7)
  ,Pos(7,0)
  ,Pos(7,7)
};
const Color Fly::colors[4] =
{
  Color(0,0xA)
  ,Color(5,0)
  ,Color(0x5,0x5)
  ,Color(0x5,0xA)
};

Fly::Fly()
  : state(Reborn)
  , count(1)
{
  id = idCount++;
  pixel.c = colors[id];
}

void
Fly::doMove(uint8_t& v)
{
  --v;
  v += random(3);
  if ( v == 0xff )
    v = 0;
  else if ( v == 8 )
    v =7;
}

void
Fly::move()
{
  switch(state)
  {
    case Reborn:
    default:
      pixel.p=corners[id];
      state = Living;
      break;
      
    case Living:
    {
      doMove(pixel.p.row);
      doMove(pixel.p.col);
    }
    break;
  }
  LedMatrix.setPixel(pixel); 
}