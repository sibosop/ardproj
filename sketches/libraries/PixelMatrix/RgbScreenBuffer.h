#ifndef RGB_SCREEN_BUFFER
#define RGB_SCREEN_BUFFER
#include <string.h>
#include <Pos.h>
#include <General.h>
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
  void clear()
  {
    memset(buffer,0,sizeof(buffer));
  }
  void setPixel(const Pos& pos, const RgbPixel& px)
  {
    buffer[pos.row % rows][pos.col % cols] = px;
    
    //DUMP(pos.row % rows);
    //DUMP(pos.col % cols);
    //DUMP(buffer[pos.row % rows][pos.col % cols].r);
    //DUMP(buffer[pos.row % rows][pos.col % cols].g);
    //DUMP(buffer[pos.row % rows][pos.col % cols].b);
  }
  void get(uint8_t r, uint8_t c, uint16_t& n, uint32_t& val)
  {
    if ( c % 2 )
    {
      n = (rows - (r+1)) + c*rows;
    }
    else
    {
      n = r + c*rows;
    }
    RgbPixel p = buffer[r][c];
    val = ((uint32_t)p.r << 16) | ((uint16_t)p.g << 8) | p.b;
  }
  void begin()
  {
    clear();
  }
  RgbPixel buffer[rows][cols];  
};
#endif