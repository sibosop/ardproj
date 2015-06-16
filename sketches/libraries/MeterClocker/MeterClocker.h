#ifndef METERCLOCKER_H
#define METERCLOCKER_H
class MeterClockerClass
{
public:
  void begin(int pin);
  void displayTime(int h,int m,int s);
private:
  int meterPin;
  
};
extern MeterClockerClass MeterClocker;
#endif