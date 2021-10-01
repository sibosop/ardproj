
#include "SoftTimer.h"
#include "RGBRamp.h"


#define NUM_PIXELS 8

RGBRamp	ramp(256);




#include <Adafruit_NeoPixel.h>
uint8_t	PIN = 7;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);




void ledTimerCallback(Task* task) {
  bool changed = false;
	ramp.step();
  
	uint32_t val = ramp.getVal();
  //Serial.println(val,HEX);
  //Serial.println(0xffffff-val,HEX);
  for (int i =0; i < 2; ++i )
	  strip.setPixelColor(i,val);
  
  if ( !changed )
    	changed = ramp.changed();

  if (changed)
     strip.show();
}



Task ledTimer(3,ledTimerCallback);

void setup() {
	randomSeed(analogRead(0));
	strip.begin();
  // Update LED contents, to start they are all 'off'
  strip.show();
  Serial.begin(9600);
  SoftTimer.add(&ledTimer);
}
