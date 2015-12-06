
#include <SoftTimer.h>
#include <General.h>

const int ValuePin = 3;
const int Dir1Pin = 4;
const int Dir2Pin = 5;
const int ChoiceButtonPin = 6;

#define FAST_TIME 50
#define CHOICE_TIME 1000
uint8_t buttTimer;
int choiceTimer;
void
makeChoice()
{
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



Task buttonTimer(3,buttonReaderCallback);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A4));
  buttTimer = FAST_TIME;
  choiceTimer = 0;
  
  pinMode(ValuePin,OUTPUT);
  pinMode(Dir1Pin,OUTPUT);
  pinMode(Dir2Pin,OUTPUT);
  pinMode(ChoiceButtonPin,INPUT_PULLUP);
  
  SoftTimer.add(&buttonTimer);
}
