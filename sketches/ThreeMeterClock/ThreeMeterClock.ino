
#include <SPI.h>
#include "General.h"
#include "RTC.h"
#include "SoftTimer.h"
#include "Adafruit_NeoPixel.h"


#define NEO_DATA_PIN 7
const int NumPixels = 8;
const int MinColor= 0;
const int MaxColor = 150;
Adafruit_NeoPixel strip(NumPixels, NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

// RTC clock:
// 	SCLK-> 13
//  MISO-> 12
//  MISI-> 11

#define RTC_INTERRUPT 2
#define SECOND_METER_PIN  3
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define RTC_CHIP_SELECT	8
#define MINUTE_METER_PIN  9
#define HOUR_METER_PIN  10
#define MAX_SECOND 240



bool ticked;
boolean tickDisable;
uint8_t  disableCount;

const uint8_t hourTable[] = 
{
  0
  ,4  //1
  ,25 //2
  ,46  //3
  ,65 //4
  ,93  //5
  ,115  //6
  ,135  //7
  ,160  //8
  ,180  //9
  ,200  //10
  ,220 //11
  ,240  //12
};
const uint8_t minTable[] =
{
  0,4,8,12,16,20,24,28,32,36       // 0
  ,40,44,48,52,56,60,64,68,72,76       // 10
  ,80,84,88,92,96,100,104,108,112,116 // 20
  ,120,124,128,132,136,140,144,148,152,156 // 30
  ,160,164,168,172,176,180,184,188,192,196 // 40
  ,200,204,208,212,216,220,224,228,232,236 // 40
  ,240
};

uint8_t lastHour = 0;
void
displayHour(uint8_t hour)
{
  hour %= 12;
  analogWrite(HOUR_METER_PIN, hourTable[hour+1]);
  if ( hour != lastHour ) {
    int r,g,b;
    lastHour = hour;
    r = random(MinColor,MaxColor);
    g = random(MinColor,MaxColor);
    b = random(MinColor,MaxColor);
    for (int i = 0; i < NumPixels; ++i) {
      strip.setPixelColor(i,r,g,b);
    }
    strip.show();
  } 
}

void
displayMinute(uint8_t min)
{
  analogWrite(MINUTE_METER_PIN, minTable[min] );
}

void
displaySecond(uint8_t sec)
{
  analogWrite(SECOND_METER_PIN, (MAX_SECOND/60)  * sec);
}
 



void
displayTime(uint8_t h, uint8_t m, uint8_t s)
{
  DUMP(h);
  DUMP(m);
  DUMP(s);
  displaySecond(s);
  displayMinute(m);
  displayHour(h);
}



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
      if ( test != 3 || !Rtc.setTimeDate(0,0,0,h-1,m,0)) {
				Serial.println(": Illegal Time");
				return;
      }	
		}
		break;	
  }
  Serial.println(" OK");
}



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
	randomSeed(analogRead(0));
  minuteTimer=hourTimer=modeTimer = 1;
  minuteCount=SLOW_TIME;
  Serial.begin(9600);
  pinMode(HOUR_METER_PIN, OUTPUT);
  pinMode(MINUTE_METER_PIN, OUTPUT);
  pinMode(SECOND_METER_PIN, OUTPUT);
  
  Rtc.init(RTC_CHIP_SELECT);
  pinMode(RTC_INTERRUPT,INPUT);
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  attachInterrupt(0,tick,RISING);
  
  randomSeed(analogRead(0));
  
  strip.begin();
  strip.show();
  
  
  SoftTimer.add(&buttonTimer);
	SoftTimer.add(&serialTimer);
  
  tickDisable = false;
  ticked = false;
}


