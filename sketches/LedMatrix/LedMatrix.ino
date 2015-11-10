
#include <SoftTimer.h>
#include <LedMatrix.h>
#include <Fly.h>
#include <Trap.h>

const int latchPin = 4;
const int resetPin = 5;
const int flyCount = 4;

Fly flies[flyCount];
Trap trap;

void patternCallback(Task* task) {
  LedMatrix.clearDisplay();
  for ( int f = 0; f < flyCount; ++f )
    flies[f].move();
  trap.display();
  LedMatrix.display();
}
Task patternTimer(80,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  LedMatrix.drawEnable = true;
}
