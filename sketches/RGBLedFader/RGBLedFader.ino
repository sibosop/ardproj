
#include "SoftTimer.h"
#include "RGBRamp.h"


#define NUM_PIXELS 40

RGBRamp	ramp;

#define NEO

#include "stripper.h"
#ifdef NEO
#include <Adafruit_NeoPixel.h>
uint8_t	PIN = 7;
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
const int fifoSize =8;
class Fifo {
private:
  uint32_t buff[fifoSize];
public:
  void clear() {
    for (int i = 0; i < fifoSize; ++i )
      buff[i] = 0;
  }
  void push(uint32_t val) {
    for (int i = 0; i < fifoSize-1; ++i )
    {
      uint32_t tmp = buff[fifoSize-2-i];
      buff[fifoSize-1-i] = tmp;
    }
    buff[0]=val;
  }
  uint32_t get(void) {
    return buff[fifoSize-1];
  }
};
Fifo fifos[NUM_PIXELS];
void ledTimerCallback(Task* task) { 
	ramp.step();
  if (ramp.changed())
  {
    uint32_t val = ramp.getVal();
    for (int i = 0; i < NUM_PIXELS; ++i ) {
      fifos[i].push(val);
      strip.setPixelColor(i,val);
      val = fifos[i].get();
    }
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
