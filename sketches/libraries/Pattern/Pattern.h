#ifndef PATTERN_H
#define PATTERN_H
#include <arduino.h>
class Pattern{
public:
	Pattern(int _min,int _max,const char *n)
		: pos(_min)
		,lastPos(_min)
		, minPos(_min)
		, maxPos(_max)
		, name(n)
	{}
		
	bool ready();
	bool done();
	const char *getName() { return name; }
	void reset(uint16_t);
	
	int pos;
	int lastPos;
private:
	int minPos;
	int maxPos;
	
	uint16_t velocity;
	bool active;
	uint16_t timer;
	const char *name;
};

#endif