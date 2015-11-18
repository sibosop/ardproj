#include "pixel.h"

const Pos zero[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,0),Pos(2,2)
  ,Pos(3,0),Pos(3,2)
  ,Pos(4,0),Pos(4,2)
  ,Pos(5,0),Pos(5,1),Pos(5,2)
  ,Pos(0xff,0xff)
};


const Pos one[] =
{
  Pos(1,1)
  ,Pos(2,0),Pos(2,1)
  ,Pos(3,1)
  ,Pos(4,1)
  ,Pos(5,0),Pos(5,1),Pos(5,2)
  ,Pos(0xff,0xff)
};

const Pos two[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,2)
  ,Pos(3,0),Pos(3,1),Pos(3,2)
  ,Pos(4,0)
  ,Pos(5,0),Pos(5,1),Pos(5,2)
  ,Pos(0xff,0xff)
};
const Pos three[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,2)
  ,Pos(3,0),Pos(3,1),Pos(3,2)
  ,Pos(4,2)
  ,Pos(5,0),Pos(5,1),Pos(5,2)
  ,Pos(0xff,0xff)
};
const Pos four[] =
{
  Pos(1,0),Pos(1,2)
  ,Pos(2,0),Pos(2,2)
  ,Pos(3,0),Pos(3,1),Pos(3,2)
  ,Pos(4,2)
  ,Pos(5,2)
  ,Pos(0xff,0xff)
};
const Pos five[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,0)
  ,Pos(3,0),Pos(3,1),Pos(3,2)
  ,Pos(4,2)
  ,Pos(5,0),Pos(5,1),Pos(5,2)
  ,Pos(0xff,0xff)
};
const Pos six[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,0)
  ,Pos(3,0),Pos(3,1),Pos(3,2)
  ,Pos(4,0),Pos(4,2)
  ,Pos(5,0),Pos(5,1),Pos(5,2)
  ,Pos(0xff,0xff)
};
const Pos seven[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,0),Pos(2,2)
  ,Pos(3,2)
  ,Pos(4,2)
  ,Pos(5,2)
  ,Pos(0xff,0xff)
};
const Pos eight[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,0),Pos(2,2)
  ,Pos(3,0),Pos(3,1),Pos(3,2)
  ,Pos(4,0),Pos(4,2)
  ,Pos(5,0),Pos(5,1),Pos(5,2)
  ,Pos(0xff,0xff)
};
const Pos nine[] =
{
  Pos(1,0),Pos(1,1),Pos(1,2)
  ,Pos(2,0),Pos(2,2)
  ,Pos(3,0),Pos(3,1),Pos(3,2)
  ,Pos(4,2)
  ,Pos(5,2)
  ,Pos(0xff,0xff)
};



const Pos *number[10] =
{
  zero,one,two,three,four,five,six,seven,eight,nine
};