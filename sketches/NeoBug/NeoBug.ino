
#include "SoftTimer.h"
#include "Bug.h"

#ifndef NUM_PIXELS
#define NUM_PIXELS 93
#endif

#ifndef NUM_BUGS
#define NUM_BUGS 5
#endif
Bug bugs[NUM_BUGS];

#include "stripper.h"

#include <Adafruit_NeoPixel.h>
uint8_t	PIN = 7;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
bool stop = false;

int counter = 0;
void ledTimerCallback(Task* task) {
  strip.clear();
#ifdef DEBUG
  if (Serial.available())
  {
    Serial.read();
    if ( stop ) {
      Serial.println("resume");
      stop = false;
    } else {
      Serial.println("stop");
      stop = true;
    }
  }
#endif
    
  if (!stop && bugManager.process()) {
    bugManager.setStrip(strip);
    strip.show();
  }
}

Task ledTimer(1,ledTimerCallback);

void setup() {
	randomSeed(analogRead(0));
	strip.begin();
  strip.show();
  
  bugManager.init();
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Bug");
#endif
  SoftTimer.add(&ledTimer);
}
