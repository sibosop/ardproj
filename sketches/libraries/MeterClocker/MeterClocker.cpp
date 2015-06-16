#include "arduino.h"
#include "MeterClocker.h"

void
MeterClockerClass::begin(int p)
{
  meterPin = 0;
  pinMode(meterPin,OUTPUT);
}

void
MeterClockerClass::displayTime(int h, int m, int s)
{
  Serial.print(h);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.print(s);
  Serial.println(" ");
}

MeterClockerClass MeterClocker;