#ifndef LED_MATRIX_H
#define LED_MATRIX_H


#include <arduino.h>
#include <SPI.h>
#include <SoftTimer.h>
#include <ScreenBuffer.h>
#include <Pixel.h>


#define MISO 12
#define MOSI 11
#define SPICLK 13

class LedMatrixClass : public Task {
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
    ColPos matrix[8];
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
  void display() { swapRequest = true; }
  void begin(byte latchPin_, byte resetPin_);
  
  void dump();
  byte maxBright;
private:
  void scanLine();
  void clearShiftRegisters();
  static void scanLineCallback(Task *);
  byte latchPin;
  byte resetPin;
  byte row;
  byte screenCnt;
};

extern LedMatrixClass LedMatrix;
#endif