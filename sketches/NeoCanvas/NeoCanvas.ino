



#include "Adafruit_NeoPixel.h"
#include "SoftTimer.h"
#include "Motion.h"


#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define NEO_DATA_PIN 7

const int NumPixels = 40;

Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

Motion c1(0,40);
Motion s1(0,40);
Motion c2(0,40);

Motion *MList[] =
{
  &c1
  ,&s1
  ,&c2
  ,0
};

int pos;
int mi;
uint8_t r,g,b;

void ledTimerCallback(Task* task) {
#if 1
  
  
  Motion *m = MList[mi];
  if ( !m->isActive() )
  {
    strip.setPixelColor(m->lastPos(),0);
    if ( MList[++mi] == 0)
      mi = 0;
    m=MList[mi];
    m->reset(random(5,30));
    r = random(0,60);
    g = random(0,60);
    b = random(0,60);
  }
  
  if(m->run())
  {
    
    strip.setPixelColor(m->lastPos(),0);
    strip.setPixelColor(m->newPos(),r,g,b);
  }
#else
  uint8_t r = random(0,60);
  uint8_t g = random(0,60);
  uint8_t b = random(0,60);
  strip.setPixelColor(NumPixels-pos,0);
  if ( ++pos == NumPixels+1 )
    pos = 1;
  strip.setPixelColor(NumPixels-pos,r,g,b);
  
#endif
  strip.show();
}
Task ledTimer(1,ledTimerCallback);

void setup() 
{
  pos = 1;
  mi = 0;
  r = random(0,60);
  g = random(0,60);
  b = random(0,60);
	randomSeed(analogRead(0));
  Serial.begin(9600);
  MList[mi]->reset(random(10,30));
  strip.begin();
  strip.show();
	SoftTimer.add(&ledTimer);
}


