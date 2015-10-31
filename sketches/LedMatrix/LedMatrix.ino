
#include <SoftTimer.h>
#include <LedMatrix.h>


const int latchPin = 4;
const int resetPin = 5;
ScreenBuffer screenBuffer;
byte red;
byte green;
byte drow;
void patternCallback(Task* task) {
  screenBuffer.clear();
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
  
  screenBuffer.buffer[drow][green].green=1;
  screenBuffer.buffer[drow][red].red=1;
  
#endif

#if 1
  for (int i = 0; i < 8; i++ )
  { 
    for ( int j = 0; j < 8; j++ )
    {
      byte c = random(4);
      switch( c )
      {
        case 1:
          screenBuffer.buffer[i][j].green = 1;
          break;
        case 2:
          screenBuffer.buffer[i][j].red = 1;
          break;
        case 3:
          screenBuffer.buffer[i][j].red = 1;
          screenBuffer.buffer[i][j].green = 1;
          break;
        default:
          break;
      }
    }
  }
#endif
  //screenBuffer.dump();
}
Task patternTimer(100,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin,&screenBuffer);
  Serial.begin(9600);
  
  SoftTimer.add(&patternTimer);
  red = 0;
  green = 0;
  drow = 0;
  randomSeed(analogRead(0));
}
