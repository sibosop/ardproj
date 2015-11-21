#include "LedMatrix.h"
#define RefreshRate 80
#define MAX_BRIGHT  32

const PaletteColor LedMatrixClass::palette[Color::MaxColors][16] =
{
  {
    PaletteColor(1,0),PaletteColor(2,0),PaletteColor(3,0),PaletteColor(4,0)
    ,PaletteColor(5,0),PaletteColor(6,0),PaletteColor(7,0),PaletteColor(8,0)
    ,PaletteColor(9,0),PaletteColor(10,0),PaletteColor(11,0),PaletteColor(12,0)
    ,PaletteColor(13,0),PaletteColor(14,0),PaletteColor(15,0),PaletteColor(16,0)
  },
  
  {
    PaletteColor(0,1),PaletteColor(0,3),PaletteColor(0,5),PaletteColor(0,7)
    ,PaletteColor(0,9),PaletteColor(0,11),PaletteColor(0,13),PaletteColor(0,15)
    ,PaletteColor(0,17),PaletteColor(0,19),PaletteColor(0,21),PaletteColor(0,23)
    ,PaletteColor(0,25),PaletteColor(0,27),PaletteColor(0,29),PaletteColor(0,31)
  },
    
  {
    PaletteColor(1,1),PaletteColor(2,2),PaletteColor(3,3),PaletteColor(4,4)
    ,PaletteColor(5,5),PaletteColor(6,6),PaletteColor(7,7),PaletteColor(8,8)
    ,PaletteColor(9,9),PaletteColor(10,10),PaletteColor(11,11),PaletteColor(12,12)
    ,PaletteColor(13,13),PaletteColor(14,14),PaletteColor(15,15),PaletteColor(16,16)
  },

  {
    PaletteColor(1,2),PaletteColor(2,4),PaletteColor(3,6),PaletteColor(4,8)
    ,PaletteColor(5,10),PaletteColor(6,12),PaletteColor(7,14),PaletteColor(8,16)
    ,PaletteColor(9,18),PaletteColor(10,20),PaletteColor(11,22),PaletteColor(12,24)
    ,PaletteColor(13,26),PaletteColor(14,28),PaletteColor(15,30),PaletteColor(16,31)
  }
};


LedMatrixClass::LedMatrixClass()
  {}

void
LedMatrixClass::setPixel(Pixel& p)
{
  if (p.c.color != Color::NullColor )
    user->buffer[p.p.row][p.p.col] = palette[p.c.color][p.c.bright];
}

void
LedMatrixClass::drawPosMap(const Pos* pm, Color c, int offset, int chaos)
{
  while ( pm->row != 0xff)
  {
    Pixel p(*pm,c);
    p.p.col += offset ;
    if ( chaos )
      p.p.col += random(chaos);
    p.p.col = p.p.col % 8;
    if ( chaos )
      p.p.row += random(chaos);
    p.p.row = p.p.row % 8;
    setPixel(p);
    ++pm;
  }
}

void
LedMatrixClass::scanLine()
{
  ColPos trans;
  bitWrite(trans.row,row,1);
  uint8_t mask = 1;
  PaletteColor *pcp = active->buffer[row];
  for (int i = 0; i < 8; i++, pcp++ )
  {
    if ( pcp->g > screenCnt )
      trans.green |= mask;
  
    if ( pcp->r > screenCnt )
      trans.red |= mask;
    mask <<= 1;
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
    }
  }

  #if 0
  Serial.println("scanLine");
  dump();
  #endif
  //trans.dump();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  clearShiftRegisters();
  digitalWrite(latchPin, LOW);
  SPI.transfer(&trans,3);
  digitalWrite(latchPin, HIGH);
  SPI.endTransaction();
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
  pinMode(latchPin, OUTPUT);
  pinMode(MOSI, OUTPUT); 
  pinMode(SPICLK, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(MISO,INPUT);
  clearShiftRegisters();
  
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