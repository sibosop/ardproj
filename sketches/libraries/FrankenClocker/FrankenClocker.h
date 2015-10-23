#ifndef FRANKENCLOCKER_H
#define FRANKENCLOCKER_H
#define COUNT_CHANGE  2

class FrankenClockerClass
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
  void setMeter(float t);
};
extern FrankenClockerClass FrankenClocker;
#endif