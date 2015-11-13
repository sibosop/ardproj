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

class PaletteColor 
{
public:
  uint8_t r;
  uint8_t g;
  PaletteColor()
    : r(0)
    , g(0)
  {}
  PaletteColor(uint8_t r_, uint8_t g_)
    : r(r_ % 0x20)
    , g(g_ % 0x20)
  {}
};

class Color
{
public:
  enum Colors {
    Red,Green,Orange,Yellow,MaxColors,NullColor
    };
  static const int maxBright = 0x20;
  Colors color;
  uint8_t bright;
  
  bool pulseDim;
  Color(Colors c, uint8_t b)
    : color(c)
    , bright(b)
    , pulseDim(false)
    {}
  Color()
    : color(NullColor)
    , bright(0)
    {}
    
  static const PaletteColor palette[MaxColors][16];
  
  inline bool dim()
  {
   
    return true;
  }
  
  inline bool brighten()
  {
    
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
};
#endif