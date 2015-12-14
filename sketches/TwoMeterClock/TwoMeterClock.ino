
#include <SPI.h>
#include "RTC.h"
#include "SoftTimer.h"
#include "General.h"


#define RTC_INTERRUPT 2
				
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define RTC_CHIP_SELECT	8
#define HOUR_METER_PIN 9
#define MINUTE_METER_PIN 10	
// RTC clock:
// 	SCLK-> 13
//  MISO-> 12
//  MISI-> 11

volatile int state = LOW;

void
displayMinute(uint8_t minute)
{
  DUMP(minute);
  analogWrite(MINUTE_METER_PIN,minute);
}

bool test = false;
void
displayHour(uint8_t hour)
{
#if 0
  hour %= 12;
  if ( hour == 0 )
	hour = 12;
  DUMP(hour);
#endif
  test = !test;
  analogWrite(HOUR_METER_PIN,test ? 60 : 0);
}

void
displayTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  Serial.println("displayTime");
  DUMP(hour);
  DUMP(minute);
  DUMP(second);
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
      if ( test != 3 || !Rtc.setTimeDate(0,0,0,h,m,0)) {
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
  Serial.println("tick");
  if ( !tickDisable ) {
    Serial.println("refresh");
    Rtc.refresh();
    displayTime(Rtc.hour,Rtc.minute,Rtc.second); 
  } else {
    if ( !disableCount || !--disableCount ) 
      tickDisable = false;
  }
}

#define FAST_TIME 50
#define SLOW_TIME 100
uint8_t  minuteTimer,minuteCount,hourTimer,modeTimer;

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
      Rtc.setTimeDate(0,0,0,Rtc.hour+1==12?0:Rtc.hour+1,Rtc.minute,0);
      Rtc.refresh();
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



void
setup() {
  minuteTimer=hourTimer=modeTimer = 1;
  minuteCount=SLOW_TIME;
  Serial.begin(9600);
  
  pinMode(RTC_INTERRUPT,INPUT);
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_METER_PIN,OUTPUT);
  pinMode(MINUTE_METER_PIN,OUTPUT);
  attachInterrupt(0,tick,RISING);
  Rtc.init(RTC_CHIP_SELECT);
  SoftTimer.add(&buttonTimer);
	SoftTimer.add(&serialTimer);
  tickDisable = false;
}


