#include <SPI.h>
#include <SoftTimer.h>
#include <LedMatrix.h>


const int latchPin = 4;
const int resetPin = 5;

byte red;
byte green;
void patternCallback(Task* task) {
  LedMatrix.clear();
  if ( ++red == 8 )
    red = 0;
  green = 7 - red;
  Serial.print("red:");Serial.println(red);
  Serial.print("green:");Serial.println(green);
  for (int i = 0; i < 8; i++ )
  {  
    LedMatrix.set(i,red,LedMatrixClass::RedPos);
    LedMatrix.set(i,green,LedMatrixClass::GreenPos);
  }
}
Task patternTimer(20000,patternCallback);

void refreshCallback(Task* task) {
  LedMatrix.refresh();
}
Task refreshTimer(3,refreshCallback);


void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&refreshTimer);
  SoftTimer.add(&patternTimer);
  red = 0;
  green = 0;
}
