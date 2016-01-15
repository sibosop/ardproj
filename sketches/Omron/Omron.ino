
#include <SoftTimer.h>
#include <SPI.h>

const int latchPin = 2;
const int resetPin = 3;


bool on;
void motorCallback(Task* task) {
  
  if ( on )
  {
    on = false;
    uint8_t reg = random(256);
    Serial.println(reg,HEX);
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    digitalWrite(latchPin, LOW);
    SPI.transfer(&reg,1);
    digitalWrite(latchPin, HIGH);
    SPI.endTransaction();
  }
  else
  {
    on = true;
    uint8_t reg = 0;
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    digitalWrite(latchPin, LOW);
    SPI.transfer(&reg,1);
    digitalWrite(latchPin, HIGH);
    SPI.endTransaction();
  }
}
Task patternTimer(1000,motorCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  pinMode(latchPin,OUTPUT);
  pinMode(resetPin,OUTPUT);
  SPI.begin();
  digitalWrite(latchPin, LOW);
  digitalWrite(resetPin,0);
  digitalWrite(resetPin,1);
  digitalWrite(latchPin, HIGH);
  
  on=true;
}
