#ifndef LED_MATRIX_H
#define LED_MATRIX_H


#include <arduino.h>
#include <SPI.h>
#include <ScreenBuffer.h>
#include <Pixel.h>
#include <TimerOne.h>
#include <BitMap.h>


#define MISO 12
#define MOSI 11
#define SPICLK 13

class LedMatrixClass {
private:

    struct ColPos {
      byte green;
      byte red;
      byte row;
      ColPos()
        : green(0)
        , red(0)
        , row(0)
        {}
      void dump()
      {
        Serial.print("row:");Serial.print(row,HEX);
        Serial.print(" red:");Serial.print(red,HEX);
        Serial.print(" green :");Serial.print(green,HEX);
        Serial.println();
      }
    };

    const BitMap *userb;
    const BitMap *activeb;
    ScreenBuffer sb1;
    ScreenBuffer sb2;
    ScreenBuffer *user;
    ScreenBuffer *active;
    bool swapRequest;
public:
  enum {
    GreenPos
    ,RedPos
  } PosType;
  
  
  LedMatrixClass();
  void clearDisplay(){user->clear();}
  void setPixel(Pixel& p);
  void display(); 
  void begin(byte latchPin_, byte resetPin_);
  void setBitMap(const BitMap* b) { userb=b; }
  
  void dump();
  byte maxBright;
  bool drawEnable;
private:
  void scanLine();
  void clearShiftRegisters();
  static void scanLineCallback();
  byte latchPin;
  byte resetPin;
  byte row;
  byte screenCnt;
  
};

extern LedMatrixClass LedMatrix;
#endif