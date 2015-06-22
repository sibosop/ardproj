#include "arduino.h"
#include "MeterClocker.h"

void
MeterClockerClass::begin(int p)
{
  meterPin = p;
  hourFlag = false;
  count = 0;
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
  if (!(++count % 2))
    hourFlag = !hourFlag;
  if ( hourFlag )
    displayHour(h);
  else
    displayMinute(m);
}

void
MeterClockerClass::displayHour(int h)
{
  h = h % 12;
  if ( h == 0 )
    h = 12;
  analogWrite(meterPin,h * 20);
}

void
MeterClockerClass::displayMinute(int m)
{
  analogWrite(meterPin,m * 4);
}

MeterClockerClass MeterClocker;