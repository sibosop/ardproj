#ifndef LED_MATRIX_H
#define LED_MATRIX_H


#include <arduino.h>
#include <SPI.h>

#define MISO 12
#define MOSI 11
#define SPICLK 13

class LedMatrixClass {
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
  
  void clear() 
  {
  #if 0
    Serial.println("clear");
  #endif
    for ( int i = 0; i < 8; ++i )
    {
        bitWrite(matrix[i].row,i,1);
        matrix[i].red = 0;
        matrix[i].green = 0;
    }
  }
  void set(byte row, byte col, int posType)
  {
  #if 0
    Serial.print("set ");
    Serial.print("row:");Serial.print(row);
    Serial.print(" col:");Serial.println(col);
  #endif
    switch (posType)
    {
      case GreenPos:
      default:
        bitWrite(matrix[row].green,col,1);
        break;
      case RedPos:
        bitWrite(matrix[row].red,col,1);
        break;
    }
  }
  void refresh(int row)
  {
    ColPos trans(matrix[row]);
    #if 0
    Serial.println("refresh");
    dump();
    #endif
    clearShiftRegisters();
    digitalWrite(latchPin, LOW);
    //digitalWrite(clockPin, LOW);
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(&trans,3);
    SPI.endTransaction();
    digitalWrite(latchPin, HIGH);
    #if 0
    dump();
    #endif
  }
  
  void begin(byte latchPin_, byte resetPin_ )
  {
    Serial.println("begin");
    clear();
    latchPin = latchPin_;
    resetPin = resetPin_;
    pinMode(latchPin, OUTPUT);
    pinMode(MOSI, OUTPUT); 
    pinMode(SPICLK, OUTPUT);
    pinMode(resetPin, OUTPUT);
    pinMode(MISO,INPUT);
    clearShiftRegisters();
    SPI.begin();
  }
  
  void dump()
  {
    Serial.println("dump");
    for (int i = 0; i < 8; ++i )
    {
      Serial.print(matrix[i].row,HEX);
      Serial.print(" ");
      Serial.print(matrix[i].red,HEX);
      Serial.print(" ");
      Serial.print(matrix[i].green,HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  
private:
  void clearShiftRegisters()
  {
    digitalWrite(latchPin, LOW);
    digitalWrite(resetPin,0);
    digitalWrite(resetPin,1);
    digitalWrite(latchPin, HIGH);
  }
  byte latchPin;
  byte resetPin;
};

extern LedMatrixClass LedMatrix;
#endif