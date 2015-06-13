/*
  BlinkTask -- using schip scheduler and ADC interrupt code
  Turns on an LED on for one second,
   then off for one second, repeatedly.
  Reads 6 averaged ADC values and sends to serial port every 43ms.

  All the useful stuff happens in the BlinkTask/BlinkCode.cpp library file.
 
  This example code is in the public domain.
 */
#include <BlinkTask.h>

// the setup routine runs once when you press reset:
void setup()
{                
	initCode();
}

// the loop routine runs over and over again forever:
void loop()
{
	// check the scheduled task list and
	//  execute any whose time has come
	scheduler();
}
