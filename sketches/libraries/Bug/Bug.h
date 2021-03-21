#ifndef BUG_H
#define BUG_H
#include "stripper.h"
#include <Adafruit_NeoPixel.h>
#include "RGB.h"

class Bug {
private:
  Adafruit_NeoPixel *strip;
  uint16_t pos;
  uint16_t maxPos;
  int speed;
  int speedCount;
  RGB color;
  const int MaxSpeed = 100;
  const int MinSpeed = 20;
public:
  Bug(){};
  void init(Adafruit_NeoPixel* strip_,int maxPos_)
  {
    pos = 0;
    strip = strip_;
    maxPos = maxPos_;
    color.reset();
    setSpeed();
  }
  bool process() {
    if ( ++speedCount < speed )
      return false;
    speedCount = 0;
    strip->setPixelColor(pos,0);
    if ( ++pos == maxPos )
    {
      pos = 0;
      color.reset();
      setSpeed();
    }
    strip->setPixelColor(pos,color.r,color.g,color.b);
    return true;
  }
private:
  void setSpeed() {
    speed = random(MinSpeed,MaxSpeed);
    speedCount = 0;
  }
};
#endif