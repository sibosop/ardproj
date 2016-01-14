
#include <SoftTimer.h>
#include <SPI.h>

const int latchPin = 2;
const int resetPin = 3;

void motorCallback(Task* task) {
}
Task patternTimer(1000,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  LedMatrix.drawEnable = true;
  coff = 0;
  up = true;
  wcnt = 0;
  val = 0;
  clr = 0;
}
