
#include "SoftTimer.h"
#include "Bug.h"



#define NUM_PIXELS 93

#define NUM_BUGS 5
Bug bugs[NUM_BUGS];

#include "stripper.h"

#include <Adafruit_NeoPixel.h>
uint8_t	PIN = 7;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int counter = 0;
void ledTimerCallback(Task* task) {
  bool doShow = false;
  for (int i = 0; i < NUM_BUGS; ++i)
  {
    if (bugs[i].process())
      doShow = true;
  }
  if (doShow)
    strip.show();
}

Task ledTimer(1,ledTimerCallback);

void setup() {
	randomSeed(analogRead(0));
	strip.begin();
  // Update LED contents, to start they are all 'off'
  strip.show();
  for (int i = 0; i < NUM_BUGS; ++i)
    bugs[i].init(&strip,NUM_PIXELS);
  Serial.begin(9600);
  SoftTimer.add(&ledTimer);
}
