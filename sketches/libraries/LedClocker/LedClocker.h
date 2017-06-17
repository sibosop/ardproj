#ifndef LED_CLOCKER_H
#define LED_CLOCKER_H

#include <Stripper.h>

class LedClocker {
public:
  LedClocker(){};
  
  enum Vars {
    NumPixels=60
    ,DataPin=3
    ,ClockPin=4
    ,MinuteColor=0x222200
    ,HourColor=0x2222
    ,ColorValid=0x5A
    ,BackgroundColor=0x020000
    ,PointColor=0x000500
  };
  void init(Stripper *strip, bool reversed=false, int offset=0);
  void resetColor();
  void displayTime(int h, int m, int s);
  void setHourColor(uint32_t c);
  void setMinuteColor(uint32_t c) ;
  void setBackgroundColor(uint32_t c);
  void setPointColor(uint32_t c);
private:
  Stripper *strip;
  int hours;
  int	minutes;
  int seconds;
  bool reversed;
  void 	clearTime();
  void	refreshTime();
  uint32_t readEepromColor(int a,uint32_t d);
  void writeEepromColor(int a, uint32_t c);
  uint32_t	minuteColor;
  uint32_t 	hourColor;
  uint32_t  backgroundColor;
  uint32_t  pointColor;
  int offset;
};

extern LedClocker	Display;

#endif


