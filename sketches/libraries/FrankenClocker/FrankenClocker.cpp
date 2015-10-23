#include "arduino.h"
#include "FrankenClocker.h"

void
FrankenClockerClass::begin(int p)
{
  meterPin = p;
  hourFlag = false;
  count = 0;
  pinMode(meterPin,OUTPUT);
}

void
FrankenClockerClass::displayTime(int h, int m, int s)
{
#if 0
  Serial.print(h);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.print(s);
  Serial.println(" ");
#endif
  ++count;
  if ( count == 2 )
  {
    count = 0;
    if (hourFlag)
    {
      hourFlag = false;
      displayHour(h);
    }
    else
    {
      hourFlag = true;
      displayMinute(m);
    }
  }
}


void
FrankenClockerClass::displayHour(int h)
{
  h = h % 12;
  if ( h == 0 )
    h = 12;
  Serial.print("hour val: ");
  Serial.println(h);
  float v = (256.0 / 12.0 ) * h;
  v -= 5.0;
  Serial.print("v: ");
  Serial.println(v);
  
  analogWrite(meterPin,v);
}

void
FrankenClockerClass::displayMinute(int m)
{
  Serial.print("minute val: ");
  Serial.println(m);
  float v = (256.0 / 120.0) * m;
  Serial.print("v: ");
  Serial.println(v);
  if ( v > 5.0 )
    v -= 5.0;
  analogWrite(meterPin,v);
}


void
FrankenClockerClass::setMeter(float meterVal)
{
  uint8_t v = (uint8_t )meterVal;
  Serial.print("meterVal:");
  Serial.println(meterVal);
  Serial.print("v:");
  Serial.println(v);
  analogWrite(meterPin,v);
}
FrankenClockerClass FrankenClocker;