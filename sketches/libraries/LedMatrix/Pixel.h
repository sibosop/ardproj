#ifndef PIXEL_H
#define PIXEL_H
class Pixel
{
public:
  byte row;
  byte col;
  byte red;
  byte green;
  Pixel()
    : row(0)
    , col(0)
    , red(0)
    , green(0)
    {}
  
  bool up()
  {
    if ( !row )
    {
      row = 7;
      return true;
    }
    --row;
    return false;
  }
  bool left()
  {
    if ( !col )
    {
      col = 7;
      return true;
    }
    --col;
    return false;
  }
  bool down()
  {
    if ( ++row == 8 )
    {
      row = 0;
      return true;
    }
    return false;
  }
  bool right()
  {
    if ( ++col == 8 )
    {   
      col = 0;
      return true;
    }
    return false;
  }
};
#endif