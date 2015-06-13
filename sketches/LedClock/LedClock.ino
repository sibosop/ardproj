#include <SPI.h>

#include "Adafruit_WS2801.h"


#include "SoftTimer.h"
#include "LedClocker.h"

Adafruit_WS2801  strip(LedClocker::NumPixels,LedClocker::DataPin,LedClocker::ClockPin);
LedClocker	ledClocker(&strip);

void ledTimerCallback(Task* task) {
  //Serial.println("time callback");
  ledClocker.tick();
}


String input;
bool inFlush = false;
void Fail() {
  Serial.print("Bad input: ");
  Serial.println(input);
  input = "";
  inFlush = true;
}
void serialReaderCallback(Task* task) {
  if ( Serial.available() > 0 ) {
    char b = Serial.read();
    if ( b == '\n' ) {
      ledClocker.doCommand(input);
      input = "";
    } else {
      input += b;
    }
  }
}
Task serialTimer(10,serialReaderCallback);
Task ledTimer(1000,ledTimerCallback);

void setup() {
  ledClocker.begin();
  randomSeed(analogRead(0));
  Serial.begin(9600);
  SoftTimer.add(&ledTimer);
  SoftTimer.add(&serialTimer);
}
