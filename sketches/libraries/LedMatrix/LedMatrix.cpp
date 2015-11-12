#include "LedMatrix.h"
#define RefreshRate 80
#define MAX_BRIGHT  32

const Color Color::palette[Color::MaxColors][16] =
{
  {
    Color(1,0),Color(2,0),Color(3,0),Color(4,0)
    ,Color(5,0),Color(6,0),Color(7,0),Color(8,0)
    ,Color(9,0),Color(10,0),Color(11,0),Color(12,0)
    ,Color(13,0),Color(14,0),Color(15,0),Color(16,0)
  },
  
  {
    Color(0,1),Color(0,3),Color(0,5),Color(0,7)
    ,Color(0,9),Color(0,11),Color(0,13),Color(0,15)
    ,Color(0,17),Color(0,19),Color(0,21),Color(0,23)
    ,Color(0,25),Color(0,27),Color(0,29),Color(0,31)
  },
    
  {
    Color(1,1),Color(2,2),Color(3,3),Color(4,4)
    ,Color(5,5),Color(6,6),Color(7,7),Color(8,8)
    ,Color(9,9),Color(10,10),Color(11,11),Color(12,12)
    ,Color(13,13),Color(14,14),Color(15,15),Color(16,16)
  },

  {
    Color(1,2),Color(2,4),Color(3,6),Color(4,8)
    ,Color(5,10),Color(6,12),Color(7,14),Color(8,16)
    ,Color(9,18),Color(10,20),Color(11,22),Color(12,24)
    ,Color(13,26),Color(14,28),Color(15,30),Color(16,31)
  }
};


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