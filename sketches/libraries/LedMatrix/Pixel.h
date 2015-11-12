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
  bool pulseDim;
  Color(uint8_t r, uint8_t g)
    : red(r)
    , green(g)
    , pulseDim(false)
    {}
  Color()
    : red(0)
    , green(0)
    {}
  inline operator bool() const
  {
    return (!red && !green);
  }
  inline bool dim()
  {
    if ( !red && !green )
      return false;
       
    if ( red == 1 || green == 1 )
    {
      return false;
    }
    
    if ( red )
      --red;
      
    if ( green )
      --green;
    return true;
  }
  
  inline bool brighten()
  {
    if ( !red && !green )
      return false;
       
    if ( red == 0x1f || green == 0x1f )
    {
      return false;
    }
    
    if ( red )
      ++red;
      
    if ( green )
      ++green;
    return true;
  }
  
  inline void pulse()
  {
    if (pulseDim)
      pulseDim = dim();
    else
      pulseDim = !brighten();
  }
};


class Pixel
{
public:
  Pos p;
  Color c;
  Pixel() {}
  Pixel(const Pos& p_, const Color& c_)
    : p(p_)
    , c(c_)
    {}
  
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