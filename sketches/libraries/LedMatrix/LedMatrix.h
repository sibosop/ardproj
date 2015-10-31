#ifndef LED_MATRIX_H
#define LED_MATRIX_H


#include <arduino.h>
#include <SPI.h>
#include <SoftTimer.h>
#include <ScreenBuffer.h>


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
public:
  enum {
    GreenPos
    ,RedPos
  } PosType;
  
  LedMatrixClass();
  void clear();
  void set(byte row, byte col, int posType);
  void scanLine();
  
  void begin(byte latchPin_, byte resetPin_,ScreenBuffer *sb);
  
  void dump();
  
private:
  void clearShiftRegisters();
  static void scanLineCallback(Task *);
  byte latchPin;
  byte resetPin;
  byte row;
  ScreenBuffer* screenBuffer;
};

extern LedMatrixClass LedMatrix;
#endif