
#include "SoftTimer.h"
#include "RGBRamp.h"


#define NUM_PIXELS 2

RGBRamp	ramp;

#define NEO

#include "stripper.h"
#ifdef NEO
#include <Adafruit_NeoPixel.h>
uint8_t	PIN = 2;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
#else
#include "Adafruit_WS2801.h"
// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
uint8_t dataPin  = 3;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 4;    // Green wire on Adafruit Pixels

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply
// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(NUM_PIXELS, dataPin, clockPin);


#endif


void ledTimerCallback(Task* task) {
  bool changed = false;
	ramp.step();
  
	uint32_t val = ramp.getVal();
//Serial.println(val,HEX);
  //Serial.println(0xffffff-val,HEX);
	strip.setPixelColor(0,val);
	strip.setPixelColor(1,val);
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
