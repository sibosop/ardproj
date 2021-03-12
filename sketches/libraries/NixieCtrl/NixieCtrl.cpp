#include "NixieCtrl.h"
#include <SPI.h>
#include <General.h>


#ifdef IN12
//                                 0,1,2,3,4,5,6,7,8,9
const int NixieCtrl::trin12[10] = {1,5,4,3,6,8,0,7,9,2};
#endif

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
#ifdef IN12
void 
NixieCtrl::set(uint8_t num, uint8_t val) { tubeVal[num] = trin12[val % 10]; }
#else
void 
NixieCtrl::set(uint8_t num, uint8_t val) { tubeVal[num] = val % 10; }
#endif

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