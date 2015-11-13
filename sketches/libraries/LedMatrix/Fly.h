#if 0
#ifndef FLY_H
#define FLY_H
#include "Pixel.h"

class Fly {
private:
  enum State {
    Reborn
    ,Living
    ,Dead
  };
  
public:
  Fly();
  void move();
private:
  State state;
  Pixel pixel;
  void doMove(uint8_t& v);
  static uint8_t idCount;
  static const Pos corners[4];
  static const Color colors[4];
  uint8_t id;
  uint8_t count;
};
#endif
#endif