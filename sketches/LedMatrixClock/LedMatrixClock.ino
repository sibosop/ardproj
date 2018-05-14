
#include <SoftTimer.h>
#include <LedMatrix.h>
#include <SPI.h>
#include "RTC.h"
#include "SoftTimer.h"

#define USE_NEON_BAR

#define RTC_INTERRUPT 2			
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define RTC_CHIP_SELECT	8

#ifdef USE_NEON_BAR
#define SECOND_METER_PIN  3
const int latchPin = 9;
#else
const int latchPin = 3;
#endif

const int resetPin = 4;
uint8_t coff = 0;
uint8_t val = 0;
bool animate = false;
uint8_t clr = 0;
uint8_t bbright;
bool up;
bool left;
uint8_t displayCount;


void fillRect(Pos ul, Pos lr, Color col)
{
  //Serial.println("enter");
  for ( int r = ul.row; r < lr.row; ++r )
  {
    for ( int c = ul.col; c < lr.col; ++c )
    {
      Pixel p(Pos(r,c),col);
      LedMatrix.setPixel(p);
    }
  }
  //Serial.println("exit");
}

void
putNumber(int num,Color c,int chaos,bool left)
{
  int ten = num/10;
  int one = num % 10;
  LedMatrix.drawPosMap(number[ten],c,left ? 0 : 1,chaos);
  LedMatrix.drawPosMap(number[one],c,left ? 4 : 5,chaos);
}

void
displayMinute(uint8_t minute)
{
  //Serial.print("m:");Serial.println(minute);
  val = minute;
  left = true;
}

void
displayHour(uint8_t hour)
{
  //Serial.print("m:");Serial.println(hour);
  left = false;
  hour %= 12;
  val = (hour ? hour : 12);
}

//const uint8_t secBase = (255-(3*59));
#define SEC_MAX 231
#define SEC_MIN 25

#define Y1 SEC_MIN
#define Y2 SEC_MAX
#define DY  (Y2-Y1)
#define X1 0
#define X2 60
#define DX  (X2-X1)



void
displaySecond(uint8_t sec)
{
#ifdef USE_NEON_BAR
  uint8_t val;
  
  //val = secBase + (sec * 3);
  val = Y1 + DY * (sec - X1) / DX;
  DUMP(sec);
  DUMP(val);
  analogWrite(SECOND_METER_PIN,val);
#endif
}


void
displayTime(uint8_t hour,uint8_t minute,uint8_t second)
{
  displaySecond(second);
  if ( --displayCount )
    return;
  
  displayCount = 2;
  if ( left )
  {
    displayHour(hour);
  }
  else
  {
    displayMinute(minute);
  }
  animate = true;
}



void patternCallback(Task* task) {
  LedMatrix.clearDisplay();
  if ( animate )
  {
    if (up)
    {
      if (++coff == 7)
      {
        up = false;
        clr = random(Color::MaxColors);
      }
    } 
    else
    {
      if ( --coff == 0 )
      {
        up = true;
        animate = false;
      }
    }
    //bbright = coff;
  }
  else
  {
    //bbright = 2;
  }
  bbright = 2;

  //Serial.println(coff);
  fillRect(Pos(0,0),Pos(8,8),Color((Color::Colors)clr,bbright));
  putNumber(val,Color((Color::Colors)(3-clr),15),coff,left);
  LedMatrix.display();
}
Task patternTimer(50,patternCallback);

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
  // done in RTC
  //SPI.begin(); 
  minuteTimer=hourTimer=modeTimer = 1;
  minuteCount=SLOW_TIME;
  pinMode(RTC_INTERRUPT,INPUT);
#ifdef USE_NEON_BAR
  pinMode(SECOND_METER_PIN,OUTPUT);
#endif
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  
  Rtc.init(RTC_CHIP_SELECT);
  gotTick = false;
#if 1
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  randomSeed(analogRead(0));
  LedMatrix.drawEnable = true;
  coff = 0;
  up = true;
  animate = false;
  val = 0;
  clr = 0;
  left = false;
  bbright = 0;
  displayCount = 2;
#endif

  attachInterrupt(0,tick,RISING);
  SoftTimer.add(&buttonTimer);
	SoftTimer.add(&serialTimer);
	SoftTimer.add(&patternTimer);
  tickDisable = false;
  
}
