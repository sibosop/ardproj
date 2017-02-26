



#include "Adafruit_NeoPixel.h"
#include "SoftTimer.h"
#include "Pattern.h"


#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define NEO_DATA_PIN 7

const int NumPixels = 40;

Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

Pattern p1(0,16,"p1",true);
Pattern p2(16,8,"p2",true);
Pattern p3(24,16,"p3",true);
Pattern p4(24,16,"p4",false);
Pattern p5(16,8,"p5",false);
Pattern p6(0,16,"p6",false);

Pattern *PList[] =
{
  &p1
  ,&p2
  ,&p3
  ,&p4
  ,&p5
  ,&p6
  ,0
};

int pos;
int mi;
uint8_t r,g,b;

void ledTimerCallback(Task* task) {
  Pattern *m = PList[mi];
  if(m->ready())
  { 
    strip.setPixelColor(m->getLastPos(),0);
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
    Serial.println(m->getPos());
#endif
    strip.setPixelColor(m->getPos(),r,g,b);
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


