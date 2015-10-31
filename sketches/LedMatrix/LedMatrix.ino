
#include <SoftTimer.h>
#include <LedMatrix.h>


const int latchPin = 4;
const int resetPin = 5;

byte red;
byte green;
byte drow;
void patternCallback(Task* task) {
  LedMatrix.clear();
#if 0
  if ( ++red == 8 )
    red = 0;
  green = red;
  for (int i = 0; i < 8; i++ )
  {  
    Serial.print("red:"); Serial.print(red,HEX);
    Serial.print(" green:"); Serial.print(green,HEX);
    Serial.println();
    if ( i % 2 )
      LedMatrix.set(i,red,LedMatrixClass::RedPos);
    else
      LedMatrix.set(i,green,LedMatrixClass::GreenPos);
  }
#endif
#if 0
  if ( ++red == 8 )
    red = 0;
  green = 7 - red;
  drow = red;
  
  
  Serial.print("drow:"); Serial.print(drow,HEX);
  Serial.print(" red:"); Serial.print(red,HEX);
  Serial.print(" green:"); Serial.println(green,HEX);
  LedMatrix.set(drow,red,LedMatrixClass::RedPos);
  LedMatrix.set(drow,green,LedMatrixClass::GreenPos);
  
#endif

#if 1
  for (int i = 0; i < 8; i++ )
  { 
    red = random(8);
    green = random(8);
    LedMatrix.set(i,red,LedMatrixClass::RedPos);
    LedMatrix.set(i,green,LedMatrixClass::GreenPos);
  }
#endif
}
Task patternTimer(100,patternCallback);

byte row;
void refreshCallback(Task* task) {
  LedMatrix.refresh(row++);
  if ( row == 8 )
    row = 0;
}
Task refreshTimer(.3,refreshCallback);


void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&refreshTimer);
  SoftTimer.add(&patternTimer);
  red = 0;
  green = 0;
  drow = 0;
  randomSeed(analogRead(0));
}
