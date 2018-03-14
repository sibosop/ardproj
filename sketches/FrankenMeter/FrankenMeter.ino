
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
const uint8_t BrightnessTable[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
  0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08,
  0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B,
  0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x10,
  0x11, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20, 
  0x21, 0x23, 0x24, 0x26, 0x27, 0x29, 0x2B, 0x2C,
  0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 
  0x3E, 0x40, 0x43, 0x45, 0x47, 0x4A, 0x4C, 0x4F,
  0x51, 0x54, 0x57, 0x59, 0x5C, 0x5F, 0x62, 0x64, 
  0x67, 0x6A, 0x6D, 0x70, 0x73, 0x76, 0x79, 0x7C,
  0x7F, 0x82, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 
  0x97, 0x9A, 0x9C, 0x9F, 0xA2, 0xA5, 0xA7, 0xAA,
  0xAD, 0xAF, 0xB2, 0xB4, 0xB7, 0xB9, 0xBB, 0xBE, 
  0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
  0xD0, 0xD2, 0xD3, 0xD5, 0xD7, 0xD8, 0xDA, 0xDB, 
  0xDD, 0xDE, 0xDF, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
  0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 
  0xED, 0xEE, 0xEF, 0xEF, 0xF0, 0xF1, 0xF1, 0xF2,
  0xF2, 0xF3, 0xF3, 0xF4, 0xF4, 0xF5, 0xF5, 0xF6, 
  0xF6, 0xF6, 0xF7, 0xF7, 0xF7, 0xF8, 0xF8, 0xF8,
  0xF9, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 
  0xFA, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFC,
  0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 
  0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
  0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 
  0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF
};

const int limit = 45;

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
#if 0
  if ( val > limit )
    analogWrite(CLOCK_LIGHT,0);
  else
    analogWrite(CLOCK_LIGHT,255-val);
#else
  analogWrite(CLOCK_LIGHT,BrightnessTable[255-val]);
#endif

  
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
