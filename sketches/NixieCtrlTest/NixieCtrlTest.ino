
#include <SoftTimer.h>
#include <SPI.h>
#include "RTC.h"
#include <TimerOne.h>
const int RefreshRate=200;
const uint8_t Blank=20;
const uint8_t NixieOnTime=100; 
bool numSet;
uint8_t blankTimer;

#define RTC_INTERRUPT 2			
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define RTC_CHIP_SELECT	8

const uint8_t NixieMult1 = 3;
const uint8_t  NixieMult2 = 4;
const uint8_t  NixieMult3 = 7;
const uint8_t  NixieMult4 = 9;

const uint8_t  NixieSelect0 = A0;
const uint8_t  NixieSelect1 = A1;
const uint8_t  NixieSelect2 = A2;
const uint8_t  NixieSelect3 = A3;

uint8_t nixieDisplay[4];
uint8_t currentTube=0;
uint8_t displayCount;
void
setNumber(uint8_t n)
{
  switch(n)
  {
    default:
    case 0:
      digitalWrite(NixieMult1,LOW);
      digitalWrite(NixieMult2,LOW);
      digitalWrite(NixieMult3,LOW);
      digitalWrite(NixieMult4,LOW);
    break;
    case 1:
      digitalWrite(NixieMult1,HIGH);
      digitalWrite(NixieMult2,LOW);
      digitalWrite(NixieMult3,LOW);
      digitalWrite(NixieMult4,LOW);
    break;
    case 2:
      digitalWrite(NixieMult1,LOW);
      digitalWrite(NixieMult2,HIGH);
      digitalWrite(NixieMult3,LOW);
      digitalWrite(NixieMult4,LOW);
    break;
    case 3:
      digitalWrite(NixieMult1,HIGH);
      digitalWrite(NixieMult2,HIGH);
      digitalWrite(NixieMult3,LOW);
      digitalWrite(NixieMult4,LOW);
    break;
    case 4:
      digitalWrite(NixieMult1,LOW);
      digitalWrite(NixieMult2,LOW);
      digitalWrite(NixieMult3,HIGH);
      digitalWrite(NixieMult4,LOW);
    break;
    case 5:
      digitalWrite(NixieMult1,HIGH);
      digitalWrite(NixieMult2,LOW);
      digitalWrite(NixieMult3,HIGH);
      digitalWrite(NixieMult4,LOW);
    break;
    case 6:
      digitalWrite(NixieMult1,LOW);
      digitalWrite(NixieMult2,HIGH);
      digitalWrite(NixieMult3,HIGH);
      digitalWrite(NixieMult4,LOW);
    break;
    case 7:
      digitalWrite(NixieMult1,HIGH);
      digitalWrite(NixieMult2,HIGH);
      digitalWrite(NixieMult3,HIGH);
      digitalWrite(NixieMult4,LOW);
    break;
    case 8:
      digitalWrite(NixieMult1,LOW);
      digitalWrite(NixieMult2,LOW);
      digitalWrite(NixieMult3,LOW);
      digitalWrite(NixieMult4,HIGH);
    break;
    case 9:
      digitalWrite(NixieMult1,HIGH);
      digitalWrite(NixieMult2,LOW);
      digitalWrite(NixieMult3,LOW);
      digitalWrite(NixieMult4,HIGH);
    break;
  }
}


void
nixieCallback()
{
  digitalWrite(NixieSelect0,LOW);
  digitalWrite(NixieSelect1,LOW);
  digitalWrite(NixieSelect2,LOW);
  digitalWrite(NixieSelect3,LOW);
  if ( ++blankTimer < Blank )
    return;

  if (blankTimer > NixieOnTime)
  {
    numSet = false;
    blankTimer = 0;
  }
    
  if ( !numSet )
  {
    numSet = true;
    switch(currentTube)
    {
      default:
      case 0:
        digitalWrite(NixieSelect0,HIGH);
      break;
      case 1:
        digitalWrite(NixieSelect1,HIGH);
      break;
      case 2:
        digitalWrite(NixieSelect2,HIGH);
      break;
      case 3:
        digitalWrite(NixieSelect3,HIGH);
      break;
    }
    if ( ++currentTube > 3)
      currentTube = 0;
  }  
}



void
displayMinute(uint8_t minute)
{
  Serial.print("m:");Serial.println(minute);
  nixieDisplay[2] = minute/10;
  nixieDisplay[3] = minute % 10;
}

void
displayHour(uint8_t hour)
{
  Serial.print("h:");Serial.println(hour);
  hour %= 12;
  nixieDisplay[0] = hour/10;
  nixieDisplay[1] = hour % 10;
}

void
displayTime(uint8_t hour,uint8_t minute,uint8_t second)
{
  displayHour(hour);
  displayMinute(minute);
}

void 
doCommand(const String& i) {
  char b[100];
	int h,m,test;
	char dummy;
  i.toCharArray(b,sizeof(b));
  //Serial.print(b);
  switch(b[0]) {
    default:
      Serial.println(": unknown command ");
      return;
			
    case 't':
		{
      test = sscanf(b,"%c %d %d",&dummy,&h,&m);
			Serial.print("hour,minute ");
			Serial.print(h,DEC);
			Serial.print(" ");
			Serial.println(m,DEC);
      if ( test != 3 ){
				Serial.println(": Illegal Time");
				return;
      }	
      noInterrupts();
      bool rval = Rtc.setTimeDate(0,0,0,h,m,0);
      interrupts();
      if (!rval)
      {
        Serial.println(": RTC can't set Time");
				return;
      }
		}
		break;
  }
  Serial.println(" OK");
}

boolean tickDisable;
uint8_t  disableCount;


// this is in the clock interrupt. 
// Interrupts will be disabled
volatile bool gotTick;
void 
tick() {
  gotTick = true;
}

#define FAST_TIME 50
#define SLOW_TIME 100
uint8_t  minuteTimer,minuteCount,hourTimer,modeTimer;

void
buttonReaderCallback(Task* task) {
  if ( gotTick )
  {
    noInterrupts();
    gotTick = false;
    if ( !tickDisable ) {
      Rtc.refresh();
      displayTime(Rtc.hour,Rtc.minute,Rtc.second); 
    } else {
      if ( !disableCount || !--disableCount ) 
        tickDisable = false;
    }
    interrupts();
  }
  if ( !digitalRead(MINUTE_BUTTON) ) {
    if ( !--minuteTimer ) {
      tickDisable = true;
      disableCount = 5;
      if ( minuteCount < 5 ) {
        ++minuteCount;
        minuteTimer = SLOW_TIME;
       } else {
        minuteTimer = FAST_TIME;
       }
       noInterrupts();
       Rtc.setTimeDate(0,0,0,Rtc.hour,Rtc.minute+1==60?0:Rtc.minute+1,0);
       Rtc.refresh();
       interrupts();
       displayMinute(Rtc.minute);
    }
  } else {
    minuteCount = 0;
    minuteTimer = 1;
  }

  if ( !digitalRead(HOUR_BUTTON) ) {
    if ( !--hourTimer ) {
      tickDisable = true;
      disableCount = 5;
      hourTimer = SLOW_TIME;
      noInterrupts();
      Rtc.setTimeDate(0,0,0,Rtc.hour+1==12?0:Rtc.hour+1,Rtc.minute,0);
      Rtc.refresh();
      interrupts();
      displayHour(Rtc.hour);
    }
  } else {
    hourTimer = 1;
  }
}


String input;
void serialReaderCallback(Task* task) {
  if ( Serial.available() > 0 ) {
    char b = Serial.read();
		Serial.print(b);
    if ( b == '\r' || b == '\n' ) {
      if ( Serial.peek() == '\n' || Serial.peek() == '\r' )
        Serial.read();
      Serial.println("");
      if ( input.length() != 0)
        doCommand(input);
      input = "";
    } else {
      input += b;
    }
  }
}

Task serialTimer(10,serialReaderCallback);
Task buttonTimer(3,buttonReaderCallback);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A4));
  // done in RTC
  //SPI.begin(); 
  minuteTimer=hourTimer=modeTimer = 1;
  minuteCount=SLOW_TIME;
  pinMode(RTC_INTERRUPT,INPUT);
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  
  pinMode(NixieMult1,OUTPUT);
  pinMode(NixieMult2,OUTPUT);
  pinMode(NixieMult3,OUTPUT);
  pinMode(NixieMult4,OUTPUT);

  pinMode(NixieSelect0,OUTPUT);
  pinMode(NixieSelect1,OUTPUT);
  pinMode(NixieSelect2,OUTPUT);
  pinMode(NixieSelect3,OUTPUT);
  setNumber(0);
  currentTube=0;
  displayMinute(0);
  displayHour(0);
  
  Rtc.init(RTC_CHIP_SELECT);
  gotTick = false;


  attachInterrupt(0,tick,RISING);
  SoftTimer.add(&buttonTimer);
	SoftTimer.add(&serialTimer);
	Timer1.initialize(RefreshRate);
  Timer1.attachInterrupt(nixieCallback);
  tickDisable = false;
  numSet = false;
}
