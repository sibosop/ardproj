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
	return pos == len;
}


void 
Pattern::reset(uint16_t v) 
{
	velocity = v;
	timer = velocity;
	pos = 0;
#if 0 
	Serial.print(name);
	Serial.print(" reset to:");
	Serial.print(pos);
	Serial.print(" start:");
	Serial.println(start);
#endif
}


int
Pattern::getPos()
{
	return calcPos(pos);
}

int
Pattern::getLastPos()
{
	return calcPos(lastPos);
}