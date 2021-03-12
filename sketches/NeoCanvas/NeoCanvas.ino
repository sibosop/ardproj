#include "General.h"
#include "Adafruit_NeoPixel.h"
#include "SoftTimer.h"
#include "Pattern.h"
#include "RotList.h"
#include "RGB.h"


//#define NEO_TEST
#ifdef NEO_TEST
#define NEO_DATA_PIN 2
const int NumPixels = 15;
const int NumRots = 1;
const int Interval = 1;
#endif

#define FIREFLY
#ifdef FIREFLY
#define NEO_DATA_PIN 7
const int NumPixels = 8;
const int NumRots = 2;
const float Interval =1;
#endif
Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

RotList<Pattern> rotList[NumRots];


RGB rgbList[NumRots];
uint16_t vList[NumRots];

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
      if ( rotList[i].atHead() )
      {
        vList[i] = random(30,100);
        rgbList[i].reset();
      }
      p->setVelocity(vList[i]);
      p->reset();
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
#ifdef NEO_TEST
  rotList[i].add( new Pattern (0, NumPixels,"p1",true));
  rotList[i].add( new Pattern (0, NumPixels,"p1",false));
#endif
#ifdef FIREFLY
  rotList[i].add( new Pattern (0, NumPixels,"p1",true));
  rotList[i].add( new Pattern (0, NumPixels,"p1",false));
#endif
#if 0
  rotList[i].add( new Pattern (0,24,"p1",true));
  rotList[i].add( new Pattern(24,16,"p2",false));
  rotList[i].add( new Pattern(24,16,"p2",true));
  rotList[i].add( new Pattern(40,8,"p3",true));
#endif
#if 0
  rotList[i].add( new Pattern (0,16,"p1",true));
  rotList[i].add( new Pattern (0,16,"p2",false));
#endif
#if 0
  rotList[i].add( new Pattern (0,8,"p1",true));
  rotList[i].add( new Pattern (0,8,"p2",false));
#endif
  rotList[i].reset();
  rgbList[i].reset();
  vList[i] = random(50,100);
}

void ledTimerCallback(Task* task) {
  bool changed = false;
  for (int i = 0; i < NumRots; ++i)
    changed = changed || testRot(i);
    
  if ( changed )
    strip.show();
}
Task ledTimer(Interval,ledTimerCallback);

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


