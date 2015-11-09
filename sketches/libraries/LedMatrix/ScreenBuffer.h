#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H
#include "Pixel.h"
class ScreenBuffer {
public:
    Color buffer[8][8];
    ScreenBuffer()
    {
      clear();
    }
    void clear()
    {
      memset(buffer,0,sizeof(buffer));
    }
};
#endif