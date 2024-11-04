
#include "SoftTimer.h"
#include "RGBRamp.h"


#define NUM_PIXELS 4

RGBRamp	ramp;

#define NEO
//#define ANAOUT

#ifdef ANAOUT
const uint8_t anpinR = 3;
const uint8_t anpinG = 5;
const uint8_t anpinB = 6;
#endif

#include "stripper.h"
#ifdef NEO
#include <Adafruit_NeoPixel.h>
//uint8_t	PIN = 2;
uint8_t	PIN = 7;  // For ArdLedRing board
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
	strip.setPixelColor(2,val);
	strip.setPixelColor(3,val);
  if ( !changed )
    	changed = ramp.changed();

  if (changed) {
#ifdef ANAOUT
    analogWrite(anpinR,(uint8_t)(val & 0xff));
    analogWrite(anpinG,(uint8_t)((val & 0xff00)>>8));
    analogWrite(anpinB,(uint8_t)((val & 0xff0000)>>16));
#endif
    strip.show();
  }
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
