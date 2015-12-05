
#include <SoftTimer.h>
#include <SPI.h>
#include <NixieCtrl.h>
#include <General.h>
#include <RTC.h>

const int nixieLatchPin = 10;
const int nixieEnablePin = 9;



#define RTC_INTERRUPT 2			
#define METER_PIN 3
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define RTC_CHIP_SELECT	8



NixieCtrl nixieCtrl(nixieLatchPin,nixieEnablePin);



void
displayMinute(uint8_t minute)
{
  //DUMP(minute);
  
  nixieCtrl.set(1,minute/10);
  nixieCtrl.set(0,minute%10);
}

void
displayHour(uint8_t hour)
{
  //DUMP(hour);
  hour %= 12;
  if ( hour == 0)
    hour = 12;
  nixieCtrl.set(3,hour/10);
  nixieCtrl.set(2,hour%10);
}


void
displaySecond(uint8_t second)
{
  analogWrite(METER_PIN, second);
}
void
displayTime(uint8_t hour,uint8_t minute,uint8_t second)
{
  displayHour(hour);
  displayMinute(minute);
  displaySecond(second);
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
      //noInterrupts();
      bool rval = Rtc.setTimeDate(0,0,0,h,m,0);
      //interrupts();
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
    //noInterrupts();
    gotTick = false;
    if ( !tickDisable ) {
      Rtc.refresh();
      displayTime(Rtc.hour,Rtc.minute,Rtc.second); 
    } else {
      if ( !disableCount || !--disableCount ) 
        tickDisable = false;
    }
    //interrupts();
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
       //noInterrupts();
       Rtc.setTimeDate(0,0,0,Rtc.hour,Rtc.minute+1==60?0:Rtc.minute+1,0);
       Rtc.refresh();
       //interrupts();
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
      //noInterrupts();
      Rtc.setTimeDate(0,0,0,Rtc.hour+1==12?0:Rtc.hour+1,Rtc.minute,0);
      Rtc.refresh();
      //interrupts();
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
  pinMode(METER_PIN,OUTPUT);
  
  displayMinute(0);
  displayHour(0);
  
  Rtc.init(RTC_CHIP_SELECT);
  gotTick = false;

  
  attachInterrupt(0,tick,RISING);
  SoftTimer.add(&buttonTimer);
	SoftTimer.add(&serialTimer);
  tickDisable = false;
  nixieCtrl.init();
}
