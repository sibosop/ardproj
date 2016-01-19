
#include <SoftTimer.h>
#include <SPI.h>
#include <Omron.h>
#include <General.h>

const int latchPin = 2;
const int resetPin = 3;
const int countMin = 100;
const int countMax = 200;

Omron omron(latchPin,resetPin);

uint8_t nextCount;
void motorCallback(Task* task) {
  if(!omron.ready(nextCount))
    return;
  if (++nextCount == omron.numMotors )
    nextCount = 0;
  if(omron.ready(nextCount))
  {
    omron.set(nextCount);
    omron.go();
  }
  
  if(omron.ready(nextCount))
  {
    if (++nextCount == Omron::numMotors)
      nextCount = 0;
  }
}
Task patternTimer(100,motorCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  pinMode(latchPin,OUTPUT);
  pinMode(resetPin,OUTPUT);
  SPI.begin();
  omron.begin();
  nextCount = 0;
  omron.set(nextCount);
  omron.go();
}
