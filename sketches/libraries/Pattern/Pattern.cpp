#include "Pattern.h"

bool 
Pattern::ready() 
{
  if (--timer == 0 )
	{
		lastPos = pos;
		++pos;
		timer = velocity;
		return true;
	} 
	return false; 
}

bool 
Pattern::done() 
{ 
	return pos == maxPos;
}


void 
Pattern::reset(uint16_t v) 
{
	velocity = v;
	timer = velocity;
	pos = minPos;
}