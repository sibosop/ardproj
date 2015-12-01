
#include "General.h"
static const int dim  = 5;
static const int lin = 1;
static const int ain = 0;

uint8_t val;
uint8_t limit;

void
setup()
{
  Serial.begin(9600);
  pinMode(dim,OUTPUT);
  pinMode(ain,INPUT);
}

void 
loop()
{

  val = (analogRead(ain)/4);
  limit = analogRead(lin)/4;
  //DUMP(val);
  //DUMP(limit);
  
  if ( val > limit )
    analogWrite(dim,0);
  else
    analogWrite(dim,255-val);
}