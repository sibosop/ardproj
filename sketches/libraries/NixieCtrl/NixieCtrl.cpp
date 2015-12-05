#include "NixieCtrl.h"
#include <SPI.h>
#include <General.h>



void 
NixieCtrl::refreshCallback(Task *t)
{
  ((NixieCtrl*)t)->refresh();
}



NixieCtrl::NixieCtrl(uint8_t latchPin_,uint8_t enablePin_)
  : Task(refreshTime,refreshCallback)
  , latchPin(latchPin_)
  , enablePin(enablePin_)
  , resetPin(0)
	, blankTimer(0)
  , tubeCnt(0)
  , mask(1)
  {
   
  }

void  
NixieCtrl::init()
{
  pinMode(latchPin,OUTPUT);
  pinMode(enablePin,OUTPUT);
  SoftTimer.add(this);
}


void  
NixieCtrl::refresh()
{
  //DUMP(blankTimer);
  if (!blankTimer++)
  {
    //Serial.println("blank start ");
    disable();
    if (++tubeCnt == numTubes )
    {
      tubeCnt = 0;
      mask = 1;
    }
    return;
  }
  if ( blankTimer == displayStart )
  {
    //DUMP(blankTimer);
    //DUMP(tubeCnt);
    uint8_t tval = tubeVal[tubeCnt];
    tval <<= 4;
    tval |= mask;
    //HDUMP(tval);
    mask <<= 1;
    SPI.beginTransaction(SPISettings(15000000, MSBFIRST, SPI_MODE0));
    digitalWrite(latchPin, LOW);
    SPI.transfer(&tval,1);
    digitalWrite(latchPin, HIGH);
    SPI.endTransaction();
    return;
  }
  if ( blankTimer == displayStart + displaySetDelay)
  {
    enable();
    return;
  }
    
  if ( blankTimer == displayEnd )
    blankTimer = 0;
}
void
NixieCtrl::disable()
{
  //Serial.println("disable");
  digitalWrite(enablePin,HIGH);
}
void
NixieCtrl::enable()
{
  //Serial.println("enable");
  digitalWrite(enablePin,LOW);
}

void
NixieCtrl::clearShiftRegisters()
{
#if 1
  digitalWrite(latchPin, LOW);
  digitalWrite(resetPin,0);
  digitalWrite(resetPin,1);
  digitalWrite(latchPin, HIGH);
#else
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(latchPin, LOW);
  uint8_t val = 0;
  SPI.transfer(&val,1);
  digitalWrite(latchPin, HIGH);
  SPI.endTransaction();
#endif
}