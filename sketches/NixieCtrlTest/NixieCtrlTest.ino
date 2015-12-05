
#include <SoftTimer.h>
#include <SPI.h>
#include <NixieCtrl.h>
#include <General.h>
const int nixieLatchPin = 10;
const int nixieClearPin = 9;
NixieCtrl nixieCtrl(nixieLatchPin,nixieClearPin);

void
nixieLoopCallback(Task *t)
{
  for ( int i = 0; i < 4; ++i )
  {
    uint8_t val = random(10);
    DUMP(val);
    nixieCtrl.set(i,val);
  }
}

Task nixieLoop(1000,nixieLoopCallback);
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A4));
  SPI.begin(); 
  SoftTimer.add(&nixieLoop);
}
