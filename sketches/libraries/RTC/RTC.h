#ifndef RTC_H
#define RTC_H
#include <stdint.h>
class RTC {
public:
	RTC (){}
	void init(uint8_t cs);
	void setHour(int h, bool pm);
	void setMinute(int m);
	bool setTimeDate(int d, int mo, int y, int h, int mi, int s);
	int day; 
	int month; 
	int year; 
	int hour; 
	int minute; 
	int second;
	bool pm;
	
	void	refresh();
	
private:
	uint8_t	cs;
};

extern RTC Rtc;
#endif