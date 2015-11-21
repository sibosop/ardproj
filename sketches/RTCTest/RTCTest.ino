#include <SPI.h>
#include "RTC.h"
#include "SoftTimer.h"
#include "General.h"

#define RTC_CHIP_SELECT	8

#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6


void
displayTime(uint8_t h, uint8_t m, uint8_t s)
{
  DUMP(h);
  DUMP(m);
  DUMP(s);
  Serial.println();
}



volatile int state = LOW;
void 
doCommand(const String& i) {
  char b[100];
  i.toCharArray(b,sizeof(b));
  Serial.print(b);
  switch(b[0]) {
    default:
      Serial.println(": unknown command ");
      return;
			
    case 't':
      char d;
      int h,m;
      int test = sscanf(b,"%c %d %d",&d,&h,&m);
      if ( test != 3 || !Rtc.setTimeDate(0,0,0,h,m,0)) {
	Serial.println(": Illegal Time");
	return;
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
       displayTime(Rtc.hour,Rtc.minute,Rtc.second);
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
      displayTime(Rtc.hour,Rtc.minute,Rtc.second);
    }
  } else {
    hourTimer = 1;
  }
}


String input;
void serialReaderCallback(Task* task) {
  if ( Serial.available() > 0 ) {
    char b = Serial.read();
    if ( b == '\n' ) {
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
  Rtc.init(RTC_CHIP_SELECT);
  //Rtc.setTimeDate( 11,12,13,10,15,00 ); 
  pinMode(2,INPUT);
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  attachInterrupt(0,tick,RISING);
  SoftTimer.add(&serialTimer);
  SoftTimer.add(&buttonTimer);
  tickDisable = false;
}


