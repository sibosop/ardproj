



#include "Adafruit_NeoPixel.h"
#include "SoftTimer.h"
#include "Pattern.h"
#include "RotList.h"


#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define NEO_DATA_PIN 7

const int NumPixels = 40;

Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);
RotList<Pattern> rotList;

uint8_t r,g,b;

void ledTimerCallback(Task* task) {
  Pattern *p = rotList.ptr();
  bool changed = false;
  if(p->ready())
  { 
    changed = true;
    strip.setPixelColor(p->getLastPos(),0);
    if ( p->done() )
    {
      p=rotList.next();
      p->reset(random(10,100));
      r = random(0,60);
      g = random(0,60);
      b = random(0,60);
    }
#if 0
    Serial.print(p->getName());
    Serial.print(":");
    Serial.println(p->getPos());
#endif
    strip.setPixelColor(p->getPos(),r,g,b);
    
  }
  if ( changed )
    strip.show();
}
Task ledTimer(1,ledTimerCallback);

void setup() 
{
  rotList.add( new Pattern (0,16,"p1",true));
  rotList.add( new Pattern(16,8,"p2",true));
  rotList.add( new Pattern(24,16,"p3",true));
  rotList.add( new Pattern(24,16,"p4",false));
  rotList.add( new Pattern(16,8,"p5",false));
  rotList.add( new Pattern(0,16,"p6",false));
  rotList.reset();
  rotList.ptr()->reset(random(10,100));
  r = random(0,60);
  g = random(0,60);
  b = random(0,60);
	randomSeed(analogRead(0));
  Serial.begin(9600);
  strip.begin();
  strip.show();
	SoftTimer.add(&ledTimer);
}


