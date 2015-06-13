
#include "arduino.h"
#include "RGBRamp.h"



RGBRamp::RGBRamp() 
	: changed_(false)
{	
	for (int i = 0; i < 3; ++i) {
		stopped[i] = false;
		curVal[i] = 0;
		curSpeed[i] = 0;
		dest[i] = random(0,256);
		speed[i] = random(LowSpeed,HighSpeed);
	}
}



void
RGBRamp::step() {
	changed_ = false;
	for ( int i = 0; i < 3; ++i ) {
		if ( ++curSpeed[i] < speed[i] ) 
			continue;
		if ( stopped[i] ) {
			stopped[i] = false;
			//Serial.print(i,HEX);
			//Serial.println(" starting");
			dest[i] = random(0,256);
			speed[i] = random(LowSpeed,HighSpeed);
			continue;
		}
		curSpeed[i] =0;
		changed_ = true;
		if ( curVal[i]  == dest[i] ) {
			if ( dest[i] == 0 ) {
				dest[i] = random(0,256);
				speed[i] = StoppedSpeed;
				stopped[i] = true;
				//Serial.print(i,HEX);
				//Serial.println(" Stopped");
			} else { 
				dest[i] = random(0,256);
				if ( dest[i] < 50 ) {
					dest[i] = 0;
				}
				speed[i] = random(LowSpeed,HighSpeed);
			}
		} else if ( curVal[i] < dest[i] ) {
			++curVal[i];
		} else {
			--curVal[i];
		}
	}
}

uint32_t
RGBRamp::getVal(void) {
	return ( ((uint32_t)curVal[0] << 16) | ((uint16_t)curVal[1] << 8) | curVal[2] );
}

