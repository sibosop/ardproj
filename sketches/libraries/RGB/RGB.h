
#ifndef RGB_H
#define RGB_H

struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  RGB() { reset(); }
  void reset()
  {
    r = random(0,60);
    g = random(0,60);
    b = random(0,60);
  }
};
#endif
