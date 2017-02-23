#ifndef MOTION_H
#define MOTION_H
#include <arduino.h>
class Motion {
public:
	Motion(int _min,int _max)
		: minPos(_min)
		, maxPos(_max)
		, pos(_min)
	{}
		
	bool run();
	bool isActive();
	int lastPos();
	int newPos();
	void reset(uint16_t);
private:
	int minPos;
	int maxPos;
	int pos;
	uint16_t velocity;
	bool active;
	uint16_t timer;
};

#endif