#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H
class ScreenBuffer {
public:
    struct Pixel
    {
      byte red;
      byte green;
      Pixel()
        : red(0)
        , green(0)
        {}
      Pixel(byte r, byte g)
        : red(r)
        , green(r)
        {}
    };
    Pixel buffer[8][8];
    void clear()
    {
      memset(buffer,0,sizeof(buffer));
    }
    void dump()
    {
      for ( int i = 0; i < 8; ++i )
      {
        Serial.print("row:");Serial.print(i);
        for ( int j = 0; j < 8; ++j )
        {
          Serial.print(" ");Serial.print(buffer[i][j].red,HEX);
          Serial.print(".");Serial.print(buffer[i][j].green,HEX);
        }
        Serial.println();
      }
      Serial.println();
    }
};
#endif