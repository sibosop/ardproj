
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
Pixel pixel[4];
uint8_t:e bCount;
const BitMap *bitMaps[] =
{
  &test3
  ,&test4
  ,&test5
  ,&test6
};

void patternCallback(Task* task) {
  LedMatrix.clearDisplay();
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

#if 0
  //Serial.print("bright:");Serial.print(bright);
  //Serial.println();
  for (int i = 0; i < 8; i++ )
  { 
    for ( int j = 0; j < 8; j++ )
    {
      byte c = random(maxRand);
      LedMatrixClass::Pixel p;
      p.row = i;
      p.col = j;
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
          p.red = random(LedMatrix.maxBright);
          p.green = random(LedMatrix.maxBright);
          break;
        default:
          break;
      }
      LedMatrix.setPixel(p);
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
#if 0
  
  pixel[0].up();
  pixel[0].left();
  pixel[1].down();
  pixel[1].right();
  pixel[2].up();
  pixel[2].right();
  pixel[3].down();
  pixel[3].left();
  for ( int i = 0; i < 4; ++i )
  {
    pixel[i].c.red = random(LedMatrix.maxBright);
    pixel[i].c.green = random(LedMatrix.maxBright);
    LedMatrix.setPixel(pixel[i]);
  }
  LedMatrix.display()
#endif
#if 1
    LedMatrix.setBitMap(bitMaps[bCount++ % 4]);
  //screenBuffer.dump();
#endif
  
}
Task patternTimer(50,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
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
  bCount = 0;
  randomSeed(analogRead(0));
  pixel[2].p.row = 7;
  pixel[3].p.col = 7;
 
}
