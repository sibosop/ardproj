
#include <SPI.h>
#include "RTC.h"
#include "General.h"
#include "Adafruit_WS2801.h"
#include "SoftTimer.h"
#include "RgbScreenBuffer.h"
// RTC clock:
// 	SCLK-> 13
//  MISO-> 12
//  MISI-> 11

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
static const uint8_t dataPin  = 7;    // Yellow wire on Adafruit Pixels
static const uint8_t clockPin = 9;    // Green wire on Adafruit Pixels
#define RTC_INTERRUPT 2
#define MINUTE_BUTTON 5
#define HOUR_BUTTON 6
#define RTC_CHIP_SELECT	8
#define NUM_BALLS 50
// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply
// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(NUM_BALLS, dataPin, clockPin);

RgbScreenBuffer rsb;


void
displayTime(uint8_t h, uint8_t m, uint8_t s)
{
  
}

uint8_t row;
uint8_t col;

void ledTimerCallback(Task* task) {
  bool changed = false;
  
  rsb.clear();
  if ( ++col == rsb.cols )
  {
    col = 0;
    if ( ++row == rsb.rows )
      row = 0;
  }
  Pos p(row,col);
  RgbPixel px(random(255),random(255),random(255));
  rsb.setPixel(p,px);
  for ( int i = 0; i < NUM_BALLS; ++i )
  {
    strip.setPixelColor(i,0);
  }
  for ( uint16_t r=0; r < rsb.rows; ++r )
  {
    for ( uint16_t c=0; c < rsb.cols; ++c )
    {
      if ( r == p.row && c == p.col )
      {
        //DUMP(r);
        //DUMP(c);
        uint16_t n;
        uint32_t val;
        rsb.get(r,c,n,val);
        //DUMP(n);
        //HDUMP(val);
        strip.setPixelColor(n,val);
      }
    }
    changed = true;
  }
  
  if (changed)
     strip.show();
}
Task ledTimer(100,ledTimerCallback);

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
		//Display.setHourColor(color);
		break;
		
		case 'm':
		test = sscanf(b,"%c %lx",&dummy, &color);
		if ( test != 2 ) {
			Serial.println(": Illegal color");
			return;
		}
		Serial.print("Set Minute Color to ");
		Serial.println(color,HEX);
		//Display.setMinuteColor(color);
		break;
		
		case 'b':
		test = sscanf(b,"%c %lx",&dummy, &color);
		if ( test != 2 ) {
			Serial.println(": Illegal color");
			return;
		}
		Serial.print("Set Background Color to ");
		Serial.println(color,HEX);
		//Display.setBackgroundColor(color);
		break;
		
		case 'p':
		test = sscanf(b,"%c %lx",&dummy, &color);
		if ( test != 2 ) {
			Serial.println(": Illegal color");
			return;
		}
		Serial.print("Set Point Color to ");
		Serial.println(color,HEX);
		//Display.setPointColor(color);
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
  rsb.begin();
  Rtc.init(RTC_CHIP_SELECT);
  
  pinMode(RTC_INTERRUPT,INPUT);
  pinMode(MINUTE_BUTTON,INPUT_PULLUP);
  pinMode(HOUR_BUTTON,INPUT_PULLUP);
  attachInterrupt(0,tick,RISING);

  SoftTimer.add(&buttonTimer);
	SoftTimer.add(&ledTimer);
	SoftTimer.add(&serialTimer);
  tickDisable = false;
  row = col = 0;
}


