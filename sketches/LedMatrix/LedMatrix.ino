
#include <SoftTimer.h>
#include <LedMatrix.h>


const int latchPin = 4;
const int resetPin = 5;
ScreenBuffer *screenBuffer;
byte red;
byte green;
byte drow;
byte bright;
int maxRand;
int randCnt;
int changeCnt;
bool changeDir;
void patternCallback(Task* task) {
  screenBuffer = LedMatrix.getScreenBuffer();
  screenBuffer->clear();
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
  
  screenBuffer->buffer[drow][green].green=1;
  screenBuffer->buffer[drow][red].red=1;
  
#endif

#if 1
  //Serial.print("bright:");Serial.print(bright);
  //Serial.println();
  for (int i = 0; i < 8; i++ )
  { 
    for ( int j = 0; j < 8; j++ )
    {
      byte c = random(maxRand);
      switch( c )
      {
#if 0
        case 2:
          screenBuffer->buffer[i][j].green = random(LedMatrix.maxBright);
          break;
        case 3:
          sscreenBuffer->buffer[i][j].red = random(LedMatrix.maxBright);
          break;
#endif
        case 1:
          screenBuffer->buffer[i][j].red = random(LedMatrix.maxBright);
          screenBuffer->buffer[i][j].green = random(LedMatrix.maxBright);
          break;
        default:
          break;
      }
    }
  }
  if ( (++randCnt % changeCnt) == 0 )
  {  
    if ( changeDir )
    {
      if ( ++maxRand == 100 )
      {
        changeDir = false;
      }
    } else {
      if (--maxRand == 2 )
      {
        changeDir = true;
      }
    }
  }
  
  
#endif
  LedMatrix.swapScreenBuffer();
  //screenBuffer.dump();
  
}
Task patternTimer(100,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  screenBuffer = LedMatrix.getScreenBuffer();
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  red = 0;
  green = 0;
  drow = 0;
  bright = 0;
  maxRand = 2;
  randCnt = 0;
  changeCnt = 3;
  changeDir = true;
  randomSeed(analogRead(0));
}
