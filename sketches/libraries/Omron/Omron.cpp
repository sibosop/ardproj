#include <Omron.h>
#include <SPI.h>
#include <General.h>

Omron::Omron(uint8_t l,uint8_t c)
  : Task(loopInterval,handler)
  , latch(l)
  , clear(c)
{}
  
void
Omron::begin()
{
  for(int i=0;i<numMotors;++i)
  {
    motorTimer[i]=0;
    readyTimer[i]=0;
    onList[i]=false;
    setList[i]=false;
  }
  SoftTimer.add((Task*)this);
}

void
Omron::set(uint8_t i)
{
  setList[i] = true;
}
  
void 
Omron::go()
{
  uint8_t mask = 0;
  for(int i = 0; i<numMotors;++i)
  {
    if (setList[i])
    {
      onList[i]=true;
      setList[i]=false;
      readyTimer[i]=readyCount;
      motorTimer[i]=offTimerCount;
      mask |= 1 <<i; 
    }
  }
  send(mask);
}
void 
Omron::clearShiftRegisters()
{
  digitalWrite(latch, LOW);
  digitalWrite(clear,0);
  digitalWrite(clear,1);
  digitalWrite(latch, HIGH);
}

void
Omron::send(uint8_t mask)
{
  clearShiftRegisters();
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(latch, LOW);
  SPI.transfer(&mask,1);
  digitalWrite(latch, HIGH);
  SPI.endTransaction();
}


void 
Omron::handler(Task*t)
{
  ((Omron *)t)->handlerLoop();
}

bool
Omron::ready(uint8_t m)
{
  return !onList[m];
}

void
Omron::handlerLoop()
{
  uint8_t mask = 0;
  bool changeFlag = false;
  for(int i=0; i<numMotors; ++i)
  {
    if ( !onList[i] )
      continue;
    if (readyTimer[i])
    { 
      --readyTimer[i];
    }
    else
    {
      onList[i]=false;
      changeFlag = true;
    }
    
    if (!motorTimer[i] || !--motorTimer[i])
      continue;
    mask |= 1 << i;
    changeFlag = true;
  }
  if(changeFlag)
    send(mask);
}