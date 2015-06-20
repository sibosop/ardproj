#ifndef METERCLOCKER_H
#define METERCLOCKER_H
#define COUNT_CHANGE  2
class MeterClockerClass
{
public:
  void begin(int pin);
  void displayTime(int h,int m,int s);
  void displayMinute(int m);
  void displayHour(int h);
private:
  int meterPin;
  int count;
  bool hourFlag;
};
extern MeterClockerClass MeterClocker;
#endif