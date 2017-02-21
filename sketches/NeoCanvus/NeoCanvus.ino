



#include "Adafruit_NeoPixel.h"
#include "SoftTimer.h"


#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define NEO_DATA_PIN 7

const int NumPixels = 16;

Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);


void ledTimerCallback(Task* task) {
  uint16_t i = random(0,NumPixels);
  uint8_t r = random(0,60);
  uint8_t g = random(0,60);
  uint8_t b = random(0,60);
  strip.setPixelColor(i,r,g,b);
  strip.show();
}
Task ledTimer(500,ledTimerCallback);

void
setup() {
	randomSeed(analogRead(0));
  Serial.begin(9600);
  strip.begin();
  strip.show();
	SoftTimer.add(&ledTimer);
}


