/*
  wiring_analog.c - analog input and output
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul

  $Id: wiring_analog.c,v 1.1 2013/11/14 04:42:36 schip Exp $
*/

#include "wiring_private.h"
#include "pins_arduino.h"

#define SCHIPCODE	// to include schip interrupt driven ADC
#define SCHIPAVG	// to also do schip's averaging of readings
#define SCHIPSKIPI2C // Skip ADC4,5 for I2C to use instead

#ifdef SCHIPCODE
// Note that the "DEFAULT" define is kinda ambiguious...kids these days....
//  I think the relevant value is DEFAULT==1,
//   which makes a anal_ref of 0100 0000b when set in the register 
uint8_t analog_reference = (DEFAULT << 6); // adjust once, use many
#else // the old way...
uint8_t analog_reference = DEFAULT;
#endif // not SCHIPCODE

void analogReference(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.
#ifdef SCHIPCODE
	// adjust value once instead of on each call....
	analog_reference = mode << 6;
#else
	analog_reference = mode;
#endif // not SCHIPCODE
}

#ifdef SCHIPCODE // interrupt driven free running ADC on all channels

// convenient way to make two bytes into one word w/o all that shiftlessness
typedef union
{
	uint16_t word;
	struct
	{
		uint8_t low;
		uint8_t high;
	};
} Uword;

// number of ADCs to convert...
//  in order to play nice with I2C on pins shared with ADC4,5
//  this will skip channels 4&5 so only 6 conversions are done
//  on each cycle through the set
#define NUM_ADCCHANNEL 8
uint8_t		_ADCNum = 0xFF;	// current conversion channel
uint16_t	_ADCValues[NUM_ADCCHANNEL];	// result storage

#ifdef SCHIPAVG
#include "../schip/scheduler.h"
#define NUM_ADCAVG	64	// number of values to average, special case value....
uint8_t		_ADCAvgCnt;	// number of readings currently accumulated
uint16_t	_ADCAccum[NUM_ADCCHANNEL];	// result accumulator for averages
uint8_t		_ADCAverage[NUM_ADCCHANNEL];	// result storage for averages

/** Return the specified ADC's averaged 8 bit value 
 **  note that someone should have gotten the ball rolling with
 **  a call to analogRead() when initializing...
 **/
uint8_t analogReadAvg(uint8_t pin)
{
	// note: convert a bunch of crap to channel numbers 0-whatever...
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__)
	if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#elif defined(analogPinToChannel) && (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__))
	pin = analogPinToChannel(pin);
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

	return _ADCAverage[pin];
}

// calculate ADC averages, set return vals, clear accumulators & counter
void doADCAverages()
{
	uint8_t i;

	for( i=0; i<NUM_ADCCHANNEL; ++i )
	{
		// note special magic of using only high byte
		//  of 64x 10-bit values to get /64 into a byte to return...
		_ADCAverage[i] = ((Uword)_ADCAccum[i]).high;
		_ADCAccum[i] = 0;
	}
	_ADCAvgCnt = 0;		// start counting all over again

	// Post a task to look at the stuff....
	postTask( 0, ADCTask, NUM_ADCCHANNEL );
}
#endif // SCHIPAVG


/** this starts a conversion on the current _ADCNum channel number
 *  results are collected by the interrupt routine which
 *  in turn re-calls this function to start the next channel
 **/
int doAnalogRead()
{
	uint8_t pin = _ADCNum;

// if we don't have an ADC don't use it...
#if defined(ADCSRA) && defined(ADCL)

	// deal with special case 2 level MUX on some chips -- probably NOT
#if defined(__AVR_ATmega32U4__)
	pin = analogPinToChannel(pin);
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#elif defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 -- collect all 10 bits in two registers (the default).
#if defined(ADMUX)
	// note that we want a 0100 0000b for the anal_ref value...
	// We have already shifted reference setting into place
	ADMUX = (analog_reference | (pin & 0x07));
	//ADMUX = (analog_reference << 6) | (pin & 0x07);
#endif

	// without a delay, we seem to read from the wrong channel
	//delay(1);	// jeez, I hope not...

	// enable interrupt
	sbi(ADCSRA, ADIE);
	// start the conversion
	sbi(ADCSRA, ADSC);

#endif // we actually have an ADC....otherwise....Why Bother(TM)

	return;
}


/** the ADC interrupt handler
 *  note NOT "ADCF_vect" !!?? (I dunno...)
 **/
SIGNAL(ADC_vect)
{
#if 1	// go ahead and execute the interrupt code for real
	Uword data; // result collection

// trace me -- toggle LED on "pin 13"
// PORTB ^= (1<<PINB5);

	// ADSC is cleared and ADIF is set when the conversion finishes
	//  but ADIF didn't work for me, so we use the old way
//	if( bit_is_set(ADCSRA, ADIF) )
	if( !bit_is_set(ADCSRA, ADSC) )
	{
		// we have to read ADCL first; doing so locks both ADCL
		// and ADCH until ADCH is read.  reading ADCL second would
		// cause the results of each conversion to be discarded,
		// as ADCL and ADCH would be locked when it completed.
		data.low  = ADCL;
		data.high = ADCH;

		// combine the two bytes and set result array
		_ADCValues[_ADCNum] = data.word;

#ifdef SCHIPAVG
		// if averaging add to our accumulator
		_ADCAccum[_ADCNum] += data.word;
#endif // SCHIPAVG

		// increment channel
		if( ++_ADCNum >= NUM_ADCCHANNEL )
		{
			// wrap around to start over
			_ADCNum = 0;
#ifdef SCHIPAVG
			// if averaging, increment number of samples
			//  then do averages when we hit our limit
			//  note that doADCAverages resets the accumulators
			if( ++_ADCAvgCnt >= NUM_ADCAVG )
				doADCAverages();
#endif // SCHIPAVG
		}
#ifdef SCHIPSKIPI2C
		else if( (_ADCNum > 3) && (_ADCNum < 6) )
		{
			// skip channels 4&5 to avoid I2C overlap
			//  note: in theory x>3<6 executes one less compare 1/2 time
			_ADCNum = 6;
		}
#endif // SCHIPSKIPI2C

		// start the next conversion
		doAnalogRead();
	}
// trace me
// PORTB ^= (1<<PINB5);

#endif // execute interrupt code
}

/** analogRead() now just returns the stored results
 *   all the conversion is done in the ADC interrupt loop
 *   ...except...
 *    on the first call this starts conversions and returns 0
 **/
int analogRead(uint8_t pin)
{
	// do we need to get the ball rolling?
	if( _ADCNum == 0xFF )
	{
#ifdef SCHIPAVG
		// if averaging, clear out the accumulators and stuff
		doADCAverages();
#endif // SCHIPAVG

		// start ADC ball rolling at channel 0
		_ADCNum == 0;
		doAnalogRead();
	}

	// note: convert a bunch of crap to channel numbers 0-whatever...
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__)
	if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#elif defined(analogPinToChannel) && (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__))
	pin = analogPinToChannel(pin);
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

	return _ADCValues[pin];
}

#else // not SCHIPCODE -- the original stuff

int analogRead(uint8_t pin)
{
	uint8_t low, high;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__)
	if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#elif defined(analogPinToChannel) && (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__))
	pin = analogPinToChannel(pin);
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif
	
#if defined(__AVR_ATmega32U4__)
	pin = analogPinToChannel(pin);
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#elif defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
	// SCHIP note -- by now we hope that "pin"
	//  has been converted to "channel" number to set the input MUX
#if defined(ADMUX)
	ADMUX = (analog_reference << 6) | (pin & 0x07);
#endif

	// without a delay, we seem to read from the wrong channel
	//delay(1);

#if defined(ADCSRA) && defined(ADCL)
	// start the conversion
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));

	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low  = ADCL;
	high = ADCH;
#else
	// we dont have an ADC, return 0
	low  = 0;
	high = 0;
#endif

	// combine the two bytes
	return (high << 8) | low;
}

#endif // not SCHIPCODE

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint8_t pin, int val)
{
	// We need to make sure the PWM output is enabled for those pins
	// that support it, as we turn it off when digitally reading or
	// writing with them.  Also, make sure the pin is in output mode
	// for consistenty with Wiring, which doesn't require a pinMode
	// call for the analog output pins.
	pinMode(pin, OUTPUT);
	if (val == 0)
	{
		digitalWrite(pin, LOW);
	}
	else if (val == 255)
	{
		digitalWrite(pin, HIGH);
	}
	else
	{
		switch(digitalPinToTimer(pin))
		{
			// XXX fix needed for atmega8
			#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
			case TIMER0A:
				// connect pwm to pin on timer 0
				sbi(TCCR0, COM00);
				OCR0 = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR0A) && defined(COM0A1)
			case TIMER0A:
				// connect pwm to pin on timer 0, channel A
				sbi(TCCR0A, COM0A1);
				OCR0A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR0A) && defined(COM0B1)
			case TIMER0B:
				// connect pwm to pin on timer 0, channel B
				sbi(TCCR0A, COM0B1);
				OCR0B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR1A) && defined(COM1A1)
			case TIMER1A:
				// connect pwm to pin on timer 1, channel A
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR1A) && defined(COM1B1)
			case TIMER1B:
				// connect pwm to pin on timer 1, channel B
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2) && defined(COM21)
			case TIMER2:
				// connect pwm to pin on timer 2
				sbi(TCCR2, COM21);
				OCR2 = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2A) && defined(COM2A1)
			case TIMER2A:
				// connect pwm to pin on timer 2, channel A
				sbi(TCCR2A, COM2A1);
				OCR2A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR2A) && defined(COM2B1)
			case TIMER2B:
				// connect pwm to pin on timer 2, channel B
				sbi(TCCR2A, COM2B1);
				OCR2B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3A1)
			case TIMER3A:
				// connect pwm to pin on timer 3, channel A
				sbi(TCCR3A, COM3A1);
				OCR3A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3B1)
			case TIMER3B:
				// connect pwm to pin on timer 3, channel B
				sbi(TCCR3A, COM3B1);
				OCR3B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR3A) && defined(COM3C1)
			case TIMER3C:
				// connect pwm to pin on timer 3, channel C
				sbi(TCCR3A, COM3C1);
				OCR3C = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR4A)
			case TIMER4A:
				//connect pwm to pin on timer 4, channel A
				sbi(TCCR4A, COM4A1);
				#if defined(COM4A0)		// only used on 32U4
				cbi(TCCR4A, COM4A0);
				#endif
				OCR4A = val;	// set pwm duty
				break;
			#endif
			
			#if defined(TCCR4A) && defined(COM4B1)
			case TIMER4B:
				// connect pwm to pin on timer 4, channel B
				sbi(TCCR4A, COM4B1);
				OCR4B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR4A) && defined(COM4C1)
			case TIMER4C:
				// connect pwm to pin on timer 4, channel C
				sbi(TCCR4A, COM4C1);
				OCR4C = val; // set pwm duty
				break;
			#endif
				
			#if defined(TCCR4C) && defined(COM4D1)
			case TIMER4D:				
				// connect pwm to pin on timer 4, channel D
				sbi(TCCR4C, COM4D1);
				#if defined(COM4D0)		// only used on 32U4
				cbi(TCCR4C, COM4D0);
				#endif
				OCR4D = val;	// set pwm duty
				break;
			#endif

							
			#if defined(TCCR5A) && defined(COM5A1)
			case TIMER5A:
				// connect pwm to pin on timer 5, channel A
				sbi(TCCR5A, COM5A1);
				OCR5A = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR5A) && defined(COM5B1)
			case TIMER5B:
				// connect pwm to pin on timer 5, channel B
				sbi(TCCR5A, COM5B1);
				OCR5B = val; // set pwm duty
				break;
			#endif

			#if defined(TCCR5A) && defined(COM5C1)
			case TIMER5C:
				// connect pwm to pin on timer 5, channel C
				sbi(TCCR5A, COM5C1);
				OCR5C = val; // set pwm duty
				break;
			#endif

			case NOT_ON_TIMER:
			default:
				if (val < 128) {
					digitalWrite(pin, LOW);
				} else {
					digitalWrite(pin, HIGH);
				}
		}
	}
}

