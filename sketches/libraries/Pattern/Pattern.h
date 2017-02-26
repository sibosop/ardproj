#ifndef PATTERN_H
#define PATTERN_H
#include <arduino.h>

class Pattern{
public:
	Pattern(int _start,int _len,const char *n,bool d)
		: pos(0)
		, start(_start)
		, len(_len)
		, name(n)
		, dir (d)
	{
		reset(defaultVelocity);
	}
		
	bool ready();
	virtual bool next();
	const char *getName() { return name; }
	virtual void reset(uint16_t);
	int getPos();
	
private:
	const int defaultVelocity = 20;
	int pos;
	int start;
	int len;
	uint16_t velocity;
	uint16_t timer;
	const char *name;
	bool dir;
};

#endif