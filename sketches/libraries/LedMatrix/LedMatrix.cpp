#include "LedMatrix.h"
#define RefreshRate 110
#define MAX_BRIGHT  32

LedMatrixClass::LedMatrixClass()
  {}

void
LedMatrixClass::setPixel(Pixel& p)
{
  user->buffer[p.p.row][p.p.col].red = p.c.red;
  user->buffer[p.p.row][p.p.col].green = p.c.green;
}

void
LedMatrixClass::scanLine()
{
  ColPos trans;
  bitWrite(trans.row,row,1);
  if ( activeb )
  {
    for (int i = 0; i < 8; i++ )
    {
      const uint8_t green = (*activeb)[row][i][1];
      if ( green > screenCnt )
        bitWrite(trans.green,i,1);
      const uint8_t red = (*activeb)[row][i][0];
      if ( red > screenCnt )
        bitWrite(trans.red,i,1);
    }
  }
  if ( drawEnable )
  {
    for (int i = 0; i < 8; i++ )
    {
      byte green = active->buffer[row][i].green;
      byte red = active->buffer[row][i].red;
      if ( green > screenCnt )
        bitWrite(trans.green,i,1);
    
      if ( red > screenCnt )
        bitWrite(trans.red,i,1);
    }
  }
  
  if (++row == 8 )
  {
    row = 0;
    if ( ++screenCnt == maxBright )
    {
      screenCnt = 0;
      if ( swapRequest )
      {
        swapRequest = false;
        ScreenBuffer *tmp = user;
        user = active;
        active = tmp; 
      }
      if ( userb )
      {
        activeb = userb;
        userb = 0;
      }
    }
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
LedMatrixClass::scanLineCallback()
{
  LedMatrix.scanLine();
}

void
LedMatrixClass::display()
{ 
  noInterrupts();
  swapRequest = true; 
  interrupts();
}

void 
LedMatrixClass::begin(byte latchPin_, byte resetPin_)
{
  maxBright = MAX_BRIGHT;
  latchPin = latchPin_;
  resetPin = resetPin_;
  row = 0;
  screenCnt = 0;
  active = &sb1;
  user = &sb2;
  activeb = 0;
  userb = 0;
  drawEnable = false;
  pinMode(latchPin, OUTPUT);
  pinMode(MOSI, OUTPUT); 
  pinMode(SPICLK, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(MISO,INPUT);
  clearShiftRegisters();
  SPI.begin();
  Timer1.initialize(RefreshRate);
  Timer1.attachInterrupt(scanLineCallback);
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