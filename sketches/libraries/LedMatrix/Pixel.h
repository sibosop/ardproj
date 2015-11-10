#ifndef PIXEL_H
#define PIXEL_H
#include <stdint.h>
class Pos
{
public:
  uint8_t row;
  uint8_t col;
  Pos(int r, int c)
    : row(r)
    , col(c)
    {}
  Pos()
    : row(0)
    , col(0)
    {}
  Pos(const Pos& p)
    : row(p.row)
    , col(p.col)
    {}
  bool operator==(const Pos& p) const
  {
    return (p.row==row && p.col==col);
  }
};

class Color
{
public:
  uint8_t red;
  uint8_t green;
  Color(uint8_t r, uint8_t g)
    : red(r)
    , green(g)
    {}
  Color()
    : red(0)
    , green(0)
    {}
  operator bool() const
  {
    return (!red && !green);
  }
};


class Pixel
{
public:
  Pos p;
  Color c;
  
  
  bool up()
  {
    if ( !p.row )
    {
      p.row = 7;
      return true;
    }
    --p.row;
    return false;
  }
  bool left()
  {
    if ( !p.col )
    {
      p.col = 7;
      return true;
    }
    --p.col;
    return false;
  }
  bool down()
  {
    if ( ++p.row == 8 )
    {
      p.row = 0;
      return true;
    }
    return false;
  }
  bool right()
  {
    if ( ++p.col == 8 )
    {   
      p.col = 0;
      return true;
    }
    return false;
  }
};
#endif