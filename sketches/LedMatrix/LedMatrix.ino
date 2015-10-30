
#include <SoftTimer.h>
#include <LedMatrix.h>


const int latchPin = 4;
const int resetPin = 5;

byte red;
byte green;
void patternCallback(Task* task) {
  LedMatrix.clear();
#if 0
  if ( ++red == 8 )
    red = 0;
  green = 7 - red;
  for (int i = 0; i < 8; i++ )
  {  
    LedMatrix.set(i,red,LedMatrixClass::RedPos);
    LedMatrix.set(i,green,LedMatrixClass::GreenPos);
  }
#else
  for (int i = 0; i < 8; i++ )
  {  
    red = random(8);
    green = random(8);
    LedMatrix.set(i,red,LedMatrixClass::RedPos);
    LedMatrix.set(i,green,LedMatrixClass::GreenPos);
  }
#endif
}
Task patternTimer(1000,patternCallback);

byte row;
void refreshCallback(Task* task) {
  LedMatrix.refresh(row++);
  if ( row == 8 )
    row = 0;
}
Task refreshTimer(1,refreshCallback);


void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&refreshTimer);
  SoftTimer.add(&patternTimer);
  red = 0;
  green = 0;
  randomSeed(analogRead(0));
}
