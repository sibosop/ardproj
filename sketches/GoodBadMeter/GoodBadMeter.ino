
#include <SoftTimer.h>
#include <General.h>

const int ValuePin = 3;
const int Dir1Pin = 4;
const int Dir2Pin = 5;
const int ChoiceButtonPin = 6;
const int LedPin = 9;

#define FAST_TIME 50
#define CHOICE_TIME 1000
uint8_t buttTimer;
int choiceTimer;
void
makeChoice()
{
  //uint8_t val = 255;
  uint8_t val = random(256);
  bool dir = random(100) > 50;
  //DUMP(val);
  //DUMP(dir);
  if ( dir )
  {
    digitalWrite(Dir1Pin,LOW);
    digitalWrite(Dir2Pin,HIGH);
  }
  else 
  {
    digitalWrite(Dir2Pin,LOW);
    digitalWrite(Dir1Pin,HIGH);
  }
  analogWrite(ValuePin,val);
  choiceTimer = CHOICE_TIME;
}

void
buttonReaderCallback(Task* task) {
  if ( choiceTimer && --choiceTimer )
    return;
  analogWrite(ValuePin,0);
  if ( !digitalRead(ChoiceButtonPin) ) {
    if ( !--buttTimer )
    {
      buttTimer = FAST_TIME;
    }
    else
    {
      makeChoice();
    }
  }
  else
  {
    buttTimer = FAST_TIME;
  }
}

uint8_t pulseVal;
bool  pulseUp;
const uint8_t pulseMax = 150;
const uint8_t pulseMin = 10;
void
ledPulser(Task *task) {
  if (choiceTimer)
  {
    pulseVal = 0;
    pulseUp  = true;
  } else if (pulseUp) {
    ++pulseVal;
    if (pulseVal == pulseMax)
      pulseUp = false;
  } else {
    if ( --pulseVal == pulseMin )
      pulseUp = true;
  }
  //Serial.println(pulseVal,DEC);
  analogWrite(LedPin,pulseVal);
}


Task buttonTimer(3,buttonReaderCallback);
Task ledTimer(10,ledPulser);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A4));
  buttTimer = FAST_TIME;
  choiceTimer = 0;
  pulseVal = 0;
  pulseUp = true;
  
  pinMode(ValuePin,OUTPUT);
  pinMode(Dir1Pin,OUTPUT);
  pinMode(Dir2Pin,OUTPUT);
  pinMode(LedPin,OUTPUT);
  pinMode(ChoiceButtonPin,INPUT_PULLUP);
  
  SoftTimer.add(&buttonTimer);
  SoftTimer.add(&ledTimer);
}
