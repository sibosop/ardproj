/*
  BlinkCode -- actual code for using schip scheduler and ADC interrupts
  Turns on an LED on for one second, then off for one second, repeatedly.
  Reads 6 averaged ADC values and sends to serial port every 43ms.
  ADCs 0-3 and 6,7 are actually read.
   4,5 are skipped because the I2C uses them.
 
  This example code is in the public domain.
 */

#include "BlinkTask.h"

#define DEFBAUD 57600 	// default baud rate for Serial port

// simple schip task to be posted
//  this function matches the PFV typedef/signature for postTask()
// wiggles LED pin and re-posts itself adinfinitum
void myTask( uint16_t arg )
{
	digitalWrite( LED, arg );   // wiggle the LED pin

	// repost this task,
	//  for 1000ms hence with inverted argument
	repostTask( 1000, !arg );

	return;
}

/** utility to initialize serial port
 *  @param baud -- baud rate to use
 */
void plotInit( unsigned int baud )
{
	Serial.begin(baud); 
	return;
}

/** utility to send a text formatted message with a newline appended
 *  @param d* -- integer data to send
 */
void plotText( int data1, int data2, int data3,
		       int data4, int data5, int data6 )
{
	// print the values as ASCII with whitespace
	Serial.print( data1 );
	Serial.print( ' ' );
	Serial.print( data2 );
	Serial.print( ' ' );
	Serial.print( data3 );
	Serial.print( ' ' );
	Serial.print( data4 );
	Serial.print( ' ' );
	Serial.print( data5 );
	Serial.print( ' ' );
	Serial.print( data6 );
	Serial.println();

	return;
}

/** Task posted when ADC averaging gets a new set of values.
 **  Posted after every 64 conversions of all 6 channels
 **    @110us/channel ~= 43ms or 23 times a second
 **  Use analogReadAvg( channel ); to get 8 bit results.
 **  Can also use analogRead( channel ); to get most recenti
 **   10 bit result with no averaging applied.
 **
 ** @param numADC -- number of channels available -- required but ignored
 **/
void ADCTask( uint16_t numADC )
{
    int values[8];
	uint8_t i;

	// ADC: read eight averaged results
	for( i=0; i<8; ++i )
	{
		// read the averaged value for each input:
		values[i] = analogReadAvg( i );
	}

	// send them as text to serial port, ignore channels 4,5
	plotText( values[0], values[1], values[2],
			   values[3], values[6], values[7] );

	return;
}

// this is called from the setup() routine
// runs once when you press reset
void initCode()
{                
	// initialize the digital pin as an output.
	pinMode( LED, OUTPUT);    
	// init plotting code
	plotInit( DEFBAUD );

	// required to start the ADC interrupt cycle, returns 0 on first call...
	//  When the interrupt is running
	//   the ADC averaging cycle will post the ADCTask() every 64 samples
	analogRead( 0 );

	// post an initial task with a delay time and a starting led value (arg)
	//  with time=0 this will execute as soon as the scheduler() is called.
	postTask( 0, myTask, HIGH ); 
}
