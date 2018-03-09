
#include <SPI.h>
#include "Stripper.h"
#include "RTC.h"

#include "Adafruit_NeoPixel.h"
#include "Adafruit_WS2801.h"
#include "LedClocker.h"
#include "SoftTimer.h"
#include "Pattern.h"
#include "RotList.h"
#include "RGB.h"

// RTC clock:
//  SCLK-> 13
//  MISO-> 12
//  MISI-> 11


#define RTC_INTERRUPT 2
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define NEO_DATA_PIN 7
#define RTC_CHIP_SELECT	8

#define BLUE_TOOTH_TX 14 // A0
#define BLUE_TOOTH_RX	15 // A1

enum RotStatus
{
  Stopped,
  Setup,
  Running
};

//#define WITH_TAIL
#ifdef WITH_TAIL
const int RotListOffset = 60;
const int NumRots = 6;
#else
const int RotListOffset = 0;
const int NumRots = 0;
#endif

const int LowVel = 10;
const int HighVel = 70;
RotList<Pattern> rotList[NumRots];
RGB rgbList[NumRots];
uint16_t vList[NumRots];
RotStatus eList[NumRots];

Adafruit_NeoPixel strip(LedClocker::NumPixels+RotListOffset
                        , NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

bool
testRot(int i)
{
  noInterrupts();
  RotStatus rs;
  rs = eList[i];
  Pattern *p = rotList[i].ptr();
  boolean changed = false;
  boolean writePixel = true;
  switch (rs)
  {
    case Stopped:
      writePixel = false;
      break;

    case Setup:
      rgbList[i].reset();
      vList[i] = random(LowVel,HighVel);
      p->setVelocity(vList[i]);
      p->reset();
      eList[i] = Running;
      break;

    case Running:
      if(p->ready())
      {
        changed = true;
        strip.setPixelColor(p->getPos(),0);
        if ( p->next() )
        {
          p=rotList[i].next();
          p->reset();
          if ( rotList[i].atHead() )
          {
            eList[i] = Stopped;
            writePixel = false;
          }
        }
      }
      else
      {
        writePixel = false;
      }
      break;
  }
  if ( writePixel )
    strip.setPixelColor(
        p->getPos()
        ,rgbList[i].r
        ,rgbList[i].g
        ,rgbList[i].b
        );
  interrupts();
  return changed;
}

void
initRot(int i){
  rotList[i].add( new Pattern (0,60,"p2",false));
  rotList[i].reset();
  rgbList[i].reset();
  vList[i] = random(LowVel,HighVel);
  eList[i] = Stopped;
}

void ledTimerCallback(Task* task) {
  bool changed = false;
  for (int i = 0; i < NumRots; ++i)
    changed = changed || testRot(i);

  if ( changed )
    strip.show();
}
Task ledTimer(1,ledTimerCallback);

volatile int state = LOW;
void 
doCommand(const String& i) {
  char b[100];
  uint32_t	color;
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
		
    case 'h':
      test = sscanf(b,"%c %lx",&dummy,&color);
      if ( test != 2 ) {
        Serial.println(": Illegal color");
        return;
      }
      Serial.print("Set Hour Color to ");
      Serial.println(color,HEX);
      Display.setHourColor(color);
    break;
    
    case 'm':
      test = sscanf(b,"%c %lx",&dummy, &color);
      if ( test != 2 ) {
        Serial.println(": Illegal color");
        return;
      }
      Serial.print("Set Minute Color to ");
      Serial.println(color,HEX);
      Display.setMinuteColor(color);
    break;
    
    case 'b':
      test = sscanf(b,"%c %lx",&dummy, &color);
      if ( test != 2 ) {
        Serial.println(": Illegal color");
        return;
      }
      Serial.print("Set Background Color to ");
      Serial.println(color,HEX);
      Display.setBackgroundColor(color);
    break;
    
    case 'p':
      test = sscanf(b,"%c %lx",&dummy, &color);
      if ( test != 2 ) {
        Serial.println(": Illegal color");
        return;
      }
      Serial.print("Set Point Color to ");
      Serial.println(color,HEX);
      Display.setPointColor(color);
    break;
  }
  Serial.println(" OK");
}

boolean tickDisable;
uint8_t  disableCount;
void 
tick() {
  if ( !tickDisable ) {
    for (int i = 0; i < NumRots; ++i)
    {
      if (eList[i] == Stopped)
      {
        eList[i] = Setup;
        break;
      }
    }
    Rtc.refresh();
    Display.displayTime(Rtc.hour,Rtc.minute,Rtc.second); 
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
       Display.displayTime(Rtc.hour,Rtc.minute,Rtc.second);
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
      Display.displayTime(Rtc.hour,Rtc.minute,Rtc.second);
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
  strip.begin();
  strip.show();
  for(int i = 0; i < NumRots; ++i)
    initRot(i);
  Display.init(&strip,false,RotListOffset);
  Rtc.init(RTC_CHIP_SELECT);
  //Rtc.setTimeDate( 11,12,13,10,15,00 ); 
  pinMode(RTC_INTERRUPT,INPUT);
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  attachInterrupt(0,tick,RISING);
  //SoftTimer.add(&serialTimer);
  SoftTimer.add(&buttonTimer);
  SoftTimer.add(&ledTimer);
  SoftTimer.add(&serialTimer);
  tickDisable = false;
}


