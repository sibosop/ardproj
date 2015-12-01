
#include <SPI.h>
#include "RTC.h"
#include "FrankenClocker.h"
#include "SoftTimer.h"
#include "General.h"


#define RTC_INTERRUPT 2
#define CLOCK_LIGHT 3
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define RTC_CHIP_SELECT	8
#define METER_PIN 9				




#define LIGHT_SENSOR A0

int limit = 45;

// RTC clock:
// 	SCLK-> 13
//  MISO-> 12
//  MISI-> 11

volatile int state = LOW;
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
      if ( test != 3 || !Rtc.setTimeDate(0,0,0,h,m,0)) {
				Serial.println(": Illegal Time");
				return;
      }	
		}
		
		break;
		
		case 'l':
		{
      test = sscanf(b,"%c %d",&dummy,&limit);
      if ( test != 2 ) {
				Serial.println(": Illegal Time");
				return;
			}
    }
    break;
    
  }
  Serial.println(" OK");
}

boolean tickDisable;
uint8_t  disableCount;

void 
tick() {
  if ( !tickDisable ) {
    Rtc.refresh();
    FrankenClocker.displayTime(Rtc.hour,Rtc.minute,Rtc.second); 
  } else {
    if ( !disableCount || !--disableCount ) 
      tickDisable = false;
  }
}

#define FAST_TIME 50
#define SLOW_TIME 100
uint8_t  minuteTimer,minuteCount,hourTimer,modeTimer;

void
sensorCallback(Task* task) {
  int val = analogRead(LIGHT_SENSOR)/4;
  //DUMP(val);
  //DUMP(limit);
  if ( val > limit )
    analogWrite(CLOCK_LIGHT,0);
  else
    analogWrite(CLOCK_LIGHT,255-val);
  
}

void
buttonReaderCallback(Task* task) {
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
       Rtc.setTimeDate(0,0,0,Rtc.hour,Rtc.minute+1==60?0:Rtc.minute+1,0);
       Rtc.refresh();
       FrankenClocker.displayMinute(Rtc.minute);
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
      Rtc.setTimeDate(0,0,0,Rtc.hour+1==12?0:Rtc.hour+1,Rtc.minute,0);
      Rtc.refresh();
      FrankenClocker.displayHour(Rtc.hour);
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
Task sensorTimer(100,sensorCallback);
Task serialTimer(10,serialReaderCallback);
Task buttonTimer(3,buttonReaderCallback);



void
setup() {
  minuteTimer=hourTimer=modeTimer = 1;
  minuteCount=SLOW_TIME;
  Serial.begin(9600);
  Rtc.init(RTC_CHIP_SELECT);
  pinMode(CLOCK_LIGHT,OUTPUT);
  pinMode(RTC_INTERRUPT,INPUT);
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  FrankenClocker.begin(METER_PIN);
  attachInterrupt(0,tick,RISING);
  SoftTimer.add(&buttonTimer);
	SoftTimer.add(&serialTimer);
	SoftTimer.add(&sensorTimer);
  tickDisable = false;
}
