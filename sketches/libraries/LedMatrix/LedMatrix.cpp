#include "LedMatrix.h"
#define RefreshRate .01
#define MAX_BRIGHT  32

LedMatrixClass::LedMatrixClass()
  : Task(RefreshRate,scanLineCallback)
  {}



void
LedMatrixClass::scanLine()
{
  ColPos trans;
  bitWrite(trans.row,row,1);
  for (int i = 0; i < 8; i++ )
  {
    byte green = screenBuffer->buffer[row][i].green;
    byte red = screenBuffer->buffer[row][i].red;
    if ( green > screenCnt )
      bitWrite(trans.green,i,1);
    
    if ( red > screenCnt )
      bitWrite(trans.red,i,1);
  }
  
  if (++row == 8 )
  {
    row = 0;
    if ( ++screenCnt == maxBright )
      screenCnt = 0;
  }
  #if 0
  Serial.println("scanLine");
  dump();
  #endif
  //trans.dump();
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

void
LedMatrixClass::scanLineCallback(Task *me)
{
  ((LedMatrixClass *)me)->scanLine();
}

void 
LedMatrixClass::begin(byte latchPin_, byte resetPin_,ScreenBuffer *sb)
{
  maxBright = MAX_BRIGHT;
  latchPin = latchPin_;
  resetPin = resetPin_;
  screenBuffer = sb;
  row = 0;
  screenCnt = 0;
  pinMode(latchPin, OUTPUT);
  pinMode(MOSI, OUTPUT); 
  pinMode(SPICLK, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(MISO,INPUT);
  clearShiftRegisters();
  SPI.begin();
  SoftTimer.add(this);
}

void 
LedMatrixClass::dump()
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


void 
LedMatrixClass::clearShiftRegisters()
{
  digitalWrite(latchPin, LOW);
  digitalWrite(resetPin,0);
  digitalWrite(resetPin,1);
  digitalWrite(latchPin, HIGH);
}
LedMatrixClass LedMatrix;