


#include "General.h"
#include "Adafruit_NeoPixel.h"
#include "SoftTimer.h"
#include "Pattern.h"
#include "RotList.h"


#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define NEO_DATA_PIN 7

const int NumPixels = 40;
const int NumRots = 8;
Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

RotList<Pattern> rotList[NumRots];

struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  RGB() { reset(); }
  void reset()
  {
    r = random(0,60);
    g = random(0,60);
    b = random(0,60);
  }
};

RGB rgbList[NumRots];

bool
testRot(int i)
{
  Pattern *p = rotList[i].ptr();
  bool changed = false;
  if(p->ready())
  { 
    changed = true;
    strip.setPixelColor(p->getPos(),0);
    if ( p->next() )
    {
      p=rotList[i].next();
      p->reset(random(50,100));
      //if ( rotList[i].atHead() )
        //rgbList[i].reset();
    }
#if 0
    Serial.print(p->getName());
    Serial.print(":");
    Serial.println(p->getPos());
#endif
    strip.setPixelColor(
      p->getPos()
      ,rgbList[i].r
      ,rgbList[i].g
      ,rgbList[i].b
    );
  }
  return changed;
}

void
initRot(int i){
  rotList[i].add( new Pattern (0,16,"p1",true));
  rotList[i].add( new Pattern(16,8,"p2",true));
  rotList[i].add( new Pattern(24,16,"p3",false));
  rotList[i].add( new Pattern(24,16,"p4",true));
  rotList[i].add( new Pattern(16,8,"p5",false));
  rotList[i].add( new Pattern(0,16,"p6",false));
  rotList[i].reset();
  rgbList[i].reset();
}

void ledTimerCallback(Task* task) {
  bool changed = false;
  for (int i = 0; i < NumRots; ++i)
    changed = changed || testRot(i);
    
  if ( changed )
    strip.show();
}
Task ledTimer(1,ledTimerCallback);

void setup() 
{
  Serial.begin(9600);
  Serial.println(freeRam());
  for ( int i = 0; i < NumRots; ++i )
    initRot(i);
  
  Serial.println(freeRam());
  Serial.print("pattern:");
  Serial.println(sizeof(Pattern));
  
	randomSeed(analogRead(0));
  
  strip.begin();
  strip.show();
	SoftTimer.add(&ledTimer);
}


