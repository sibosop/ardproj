#include "Motion.h"

bool 
Motion::run() 
{
  if (--timer == 0 )
	{
		timer = velocity;
		return true;
	} 
	return false; 
}

bool 
Motion:: isActive() 
{ 
	return active; 
}
int 
Motion:: lastPos() 
{ 
	return pos; 
}
int 
Motion:: newPos() 
{ 
	if ( ++pos == maxPos )
	{
		pos = minPos;
		active = false;
	}
	return pos;
}

void 
Motion:: reset(uint16_t v) 
{
	active = true;
	velocity = v;
	timer = velocity;
	pos = 0;
}