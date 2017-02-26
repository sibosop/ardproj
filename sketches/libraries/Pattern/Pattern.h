#ifndef PATTERN_H
#define PATTERN_H
#include <arduino.h>
class Pattern{
public:
	Pattern(int _start,int _len,const char *n,bool d)
		: pos(0)
		, lastPos(0)
		, start(_start)
		, len(_len)
		, name(n)
		, dir (d)
	{}
		
	bool ready();
	bool done();
	const char *getName() { return name; }
	void reset(uint16_t);
	int getPos();
	int getLastPos();
	
private:
	int pos;
	int lastPos;
	int start;
	int len;
	
	uint16_t velocity;
	bool active;
	uint16_t timer;
	inline int calcPos(int p)
	{
		int rval = p;
		if ( dir )
			rval = start+p;
		else
			rval = (start+len-1) - p;
		return rval;
	}
	
	const char *name;
	bool dir;
};

#endif