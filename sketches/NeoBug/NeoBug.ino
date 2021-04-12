
#include "SoftTimer.h"
#include "Bug.h"

#ifndef NUM_PIXELS
#define NUM_PIXELS 93
#endif

#define NUM_BUGS 5
Bug bugs[NUM_BUGS];

#include "stripper.h"

#include <Adafruit_NeoPixel.h>
uint8_t	PIN = 7;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int counter = 0;
void ledTimerCallback(Task* task) {
  strip.clear();
  if (bugManager.process())
    strip.show();
}

Task ledTimer(1,ledTimerCallback);

void setup() {
  
	randomSeed(analogRead(0));
	strip.begin();
  strip.show();
  Serial.begin(9600);
  bugManager.init();
  SoftTimer.add(&ledTimer);
}
