/*
  BlinkTask.h -- library interface file for Blink test program
  	using schip's scheduler and ADC interrupt core mods.

   The scheduler maintains a short list of functions to execute,
   each with a single int sized argument (which can be coerced to
   a 64Kb pointer if one likes), and the number of timer ticks (milli-sec)
   until it should be executed. The list is currently defined with
   a length of 8 items. The timer tick interrupt decrements the
   timer values on each pass. The standard Arduino loop() function
   contains (only) a call to the schip scheduler() function which
   checks for timer values of zero and then executes the relevant
   function with the given argument. The function can do anything
   it wants, as long as it doesn't block or delay(), and can repost
   itself (or other Tasks).

   The ADC interrupt code free runs and accesses each of 6 ADCs in
   sequence on a period of about 110 micro-sec per channel. Channels
   4,5 are skipped because they are also used by the I2C interface,
   so only 0-3 and 6,7 are converted. The most recent result is
   accessed immediately using the 'regular' analogRead() call.

   Additionally the ADC code calculates an 8 bit average for each
   channel over 64 samples. At the end of each average cycle it
   posts the ADCTask() which the scheduler executes in "user space"
   (not as an interrupt). This task is posted about 23 times a second
   (about 43 milli-sec per) and can be used to drive most
   human-interface-rate data collection schemes. The averaged ADC
   values are accessed with the function: analogReadAvg(i) where
   'i' is the channel number 0-3,6,7. The period of this average
   makes it useful for minimizing 50-60hz hum amongst other noises.

 To use:

   First check Arduino/cores/arduino/*.c files against your version.
    The files provided are based on version 1.04 and the originals
	are included to compare and contrast to what you have...
	Make changes as needed.

   Then copy the directory contents under Arduino/cores to your system:
     arduino-1.0.4/hardware/arduino/cores
   This will replace the standard cores/arduino files with schip's and
   create the parallel cores/schip directory with the scheduler code.

   Then open libraries/BlinkTask/BlinkTask/BlinkTask.ino with the
   regular Arduino IDE and notice that it contains almost nothing.

   Then open libraries/BlinkTask/BlinkCode to see what it really does.
   You can use any editor or IDE you like for this.

   Use the Arduino IDE to compile, download, and look at text data
   sent to the serial port just as you would normally. But never use
   it to edit code again...

  This code is in some domain.
 */
#ifndef BLINKTASK_h
#define BLINKTASK_h

#include "Arduino.h"

// get the prototypes for the schip-scheduler
//  (hidden in arduino-1.0.4/hardware/arduino/cores/schip
//   and accessed from default cores/arduino directory)
#include "../schip/scheduler.h"
 
#ifndef LED
// Pin 13 has an LED connected on most Arduino boards.
#define LED		 13	// call it out
#endif // not LED

// the only external function in this library
extern void initCode(void);

#endif // BLINKTASK_h
