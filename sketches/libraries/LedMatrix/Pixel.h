#ifndef PIXEL_H
#define PIXEL_H
class Pos
{
public:
  byte row;
  byte col;
  Pos(int r, int c)
    : row(r)
    , col(c)
    {}
  Pos()
    : row(0)
    , col(0)
    {}
};

class Color
{
public:
  byte red;
  byte green;
  Color(byte r, byte g)
    : red(r)
    , green(g)
    {}
  Color()
    : red(0)
    , green(0)
    {}
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