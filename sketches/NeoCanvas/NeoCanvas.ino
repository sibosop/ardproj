



#include "Adafruit_NeoPixel.h"
#include "SoftTimer.h"
#include "Pattern.h"


#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define NEO_DATA_PIN 7

const int NumPixels = 40;

Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

Pattern c1(0,16,"c1");
Pattern s1(16,24,"s1");
Pattern c2(24,40,"c2");

Pattern *PList[] =
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
  Pattern *m = PList[mi];
  if(m->ready())
  { 
    strip.setPixelColor(m->lastPos,0);
    if ( m->done() )
    {
      if ( PList[++mi] == 0)
        mi = 0;
      m=PList[mi];
      m->reset(random(10,100));
      r = random(0,60);
      g = random(0,60);
      b = random(0,60);
    }
#if 0
    Serial.print(m->getName());
    Serial.print(":");
    Serial.println(m->pos);
#endif
    strip.setPixelColor(m->pos,r,g,b);
    strip.show();
  }
  
}
Task ledTimer(1,ledTimerCallback);

void setup() 
{
  
  mi = 0;
  r = random(0,60);
  g = random(0,60);
  b = random(0,60);
	randomSeed(analogRead(0));
  Serial.begin(9600);
  PList[mi]->reset(random(10,30));
  strip.begin();
  strip.show();
	SoftTimer.add(&ledTimer);
}


