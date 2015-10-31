#ifndef LED_MATRIX_H
#define LED_MATRIX_H


#include <arduino.h>
#include <SPI.h>
#include <SoftTimer.h>


#define MISO 12
#define MOSI 11
#define SPICLK 13

class LedMatrixClass : public Task {
private:
    struct ColPos {
      byte green;
      byte red;
      byte row;
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
  
  void begin(byte latchPin_, byte resetPin_ );
  
  void dump();
  
private:
  void clearShiftRegisters();
  static void scanLineCallback(Task *);
  byte latchPin;
  byte resetPin;
  byte row;
};

extern LedMatrixClass LedMatrix;
#endif