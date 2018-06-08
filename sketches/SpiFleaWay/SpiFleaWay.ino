// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

#include <SPI.h>

char buf [100];
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
  Serial.begin (9600);   // debugging
  Serial.println("setup");
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  buf[pos] = SPDR;  // grab byte from SPI Data Register
  if (buf[pos] == '\n')
  {
    buf[pos] = 0;
    process_it = true;
  }
  pos++;
}  // end of interrupt routine SPI_STC_vect

unsigned long count = 0;
long last = 0;
boolean first = true;
int errCnt = 0;

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (process_it)
    { 
      long t = strtol(buf,NULL,16);
      if ( first ) {
        last = t;
        first= false;
      } else {
#if 0
        Serial.print("t:");
        Serial.print(t,HEX);
        Serial.print(" last:");
        Serial.println(last,HEX);
#endif
        if (last != (t-1) ) {
          Serial.print("Dis last:");
          Serial.print(last,HEX);
          Serial.print(" t:");
          Serial.println(t,HEX);
          errCnt++;
        }
      }
      last = t;
      process_it = false;
      pos = 0;
      if ( !(++count % 2000) ) {
        Serial.print("count:");
        Serial.print(count,DEC);
        Serial.print(" errs:");
        Serial.println(errCnt,DEC);
      }
    }  // end of flag set
    
}  // end of loop
