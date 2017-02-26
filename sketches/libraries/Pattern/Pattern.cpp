#include "Pattern.h"

bool 
Pattern::ready() 
{
  if (--timer == 0 )
	{
		
		timer = velocity;
		return true;
	} 
	return false; 
}

bool 
Pattern::next() 
{ 
	return ++pos == len;
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
	int rval;
	if ( dir )
		rval = start+pos;
	else
		rval = (start+len-1) - pos;
	return rval;

}
