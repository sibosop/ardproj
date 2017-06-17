

#include "LedClocker.h"
#include <EEPROM.h>

uint32_t
LedClocker::readEepromColor(int a,uint32_t defaultColor) {
  uint32_t r = 0;
  if ( EEPROM.read(a+3) == ColorValid ) {
      for ( int i = 0; i < 3; ++i ) {
              uint32_t v = ((uint32_t)EEPROM.read(a+i)) << (i*8);
              r |= v;
              Serial.print("a+i,v,r ");
              Serial.print(a+i);
              Serial.print(" ");
              Serial.print(v,HEX);
              Serial.print(" ");
              Serial.println(r,HEX);
      }
  } else {
        Serial.println("Color Valid not set");
        r = defaultColor;
  }
  Serial.print("readEepromColor color 0x");
  Serial.println(r,HEX);
  return r;
}

void
LedClocker::writeEepromColor(int a, uint32_t c) {
	int i;
	for ( i = 0; i < 3; ++i ) {
		uint8_t v = (c >> (i*8)) & 0xff;
		Serial.print("a+i,v ");
		Serial.print(a+i);
		Serial.print(" ");
		Serial.println(v,HEX);
		EEPROM.write(a+i, v);
	}
	EEPROM.write(a+i,ColorValid);
}

void
LedClocker::resetColor() {
	setHourColor(HourColor);
	setMinuteColor(MinuteColor);
}

void
LedClocker::setHourColor(uint32_t c) {
	hourColor = c;
	writeEepromColor(4,hourColor);
}

void
LedClocker::setMinuteColor(uint32_t c) {
	minuteColor = c;
	writeEepromColor(0,minuteColor);
}

void
LedClocker::setBackgroundColor(uint32_t c) {
	backgroundColor = c;
	writeEepromColor(8,backgroundColor);
}

void
LedClocker::setPointColor(uint32_t c) {
	pointColor = c;
	writeEepromColor(12,pointColor);
}

void
LedClocker::init(Stripper *strip_,bool reversed_,int offset_) {
  strip = strip_;
  reversed=reversed_;
  offset = offset_;
  hours = 0;
  minutes = 0;
  seconds = 0;
  minuteColor = MinuteColor;
  hourColor = HourColor;
  backgroundColor = BackgroundColor;
  pointColor = PointColor;
	
  minuteColor = readEepromColor(0,MinuteColor);
  hourColor = readEepromColor(4,HourColor);
  backgroundColor = readEepromColor(8,BackgroundColor);
  pointColor = readEepromColor(12,PointColor);
}

void
LedClocker::displayTime(int h, int m, int s) {
  clearTime();
  hours = h % 12;
  minutes = m;
  seconds = s;
  Serial.print("h:");Serial.print(h);
  Serial.print(" m:");Serial.print(m);
  Serial.print(" s:");Serial.print(s);
  Serial.println();
  refreshTime();
}



void
LedClocker::clearTime() {
  if ( reversed ) {
    strip->setPixelColor(((60-seconds)%60)+offset,0);
    strip->setPixelColor(((60-minutes)%60)+offset,0);
    strip->setPixelColor(((60-((hours*5)+(minutes/12)))%60)+offset,0);
  } else {
    strip->setPixelColor(seconds+offset,0);
    strip->setPixelColor(minutes+offset,0);
    strip->setPixelColor(((hours*5)+(minutes/12))+offset,0);
  }
}

void
LedClocker::refreshTime() {
  int r,g,b;
  r = random(0,30);
  g = random(0,30);
  b = random(0,30);
  int mpos,hpos,spos;
  if ( reversed ) {
    mpos=(60-minutes)%60;
    hpos=(60-((hours*5)+(minutes/12)))%60;
    spos=(60-seconds)%60;
  } else {
    mpos=minutes;
    hpos=(hours*5)+(minutes/12);
    spos=seconds;
  }
  mpos += offset;
  hpos += offset;
  spos += offset;
  for ( int i = offset; i < (60+offset); ++i )
    strip->setPixelColor(i,backgroundColor);
  for ( int i = 0; i < 12; ++i )
    strip->setPixelColor((i*5)+offset,pointColor);

  strip->setPixelColor(spos,r,g,b);
  strip->setPixelColor(mpos,minuteColor);
  strip->setPixelColor(hpos,hourColor);
  
  strip->show();
}




LedClocker	Display;
