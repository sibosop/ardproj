#ifndef RGB_SCREEN_BUFFER
#define RGB_SCREEN_BUFFER
#include <string.h>
#include <Pos.h>
#include <General.h>
#include "Adafruit_WS2801.h"
class RgbPixel
{
public:
  uint8_t r;
  uint8_t g;
  uint8_t b;
  
  RgbPixel()
    :r(0),g(0),b(0)
  {}
  RgbPixel(uint8_t r_, uint8_t g_, uint8_t b_)
    : r(r_)
    , g(g_)
    , b(b_)
    {}
};

class RgbScreenBuffer
{
public:
  static const int rows = 5;
  static const int cols = 10;
  RgbScreenBuffer(uint8_t dataPin, uint8_t clockPin);
  inline void clear()
  {
    for ( int i = 0; i < (rows*cols); ++i )
    {
      strip.setPixelColor(i,0);
    }
    changed = true;
  }
  inline void setPixel(const Pos& pos, const RgbPixel& px)
  {
    uint16_t n;
    if ( pos.col % 2 )
    {
      n = (rows - (pos.row+1)) + pos.col*rows;
    }
    else
    {
      n = pos.row+ pos.col*rows;
    }
    uint32_t val = ((uint32_t)px.r << 16) | ((uint16_t)px.g << 8) | px.b;
    strip.setPixelColor(n,val);
    changed = true;
    
    //DUMP(pos.row % rows);
    //DUMP(pos.col % cols);
    //DUMP(buffer[pos.row % rows][pos.col % cols].r);
    //DUMP(buffer[pos.row % rows][pos.col % cols].g);
    //DUMP(buffer[pos.row % rows][pos.col % cols].b);
  }
  
  void begin()
  {
    strip.begin();
    changed = false;
    clear();
  }
  
  void show()
  {
    if ( changed )
    {
      strip.show();
      changed = false;
    }
  }
  //RgbPixel buffer[rows][cols];  
private:
  Adafruit_WS2801 strip;
  bool changed;
};
#endif