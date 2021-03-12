// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

#include <SPI.h>
#include <QueueArray.h>


QueueArray<char> queue;

void setup (void)
{
  Serial.begin (9600);   // debugging
  Serial.println("setup");
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  char c = SPDR;
  queue.enqueue(c);  // grab byte from SPI Data Register
}  // end of interrupt routine SPI_STC_vect

uint8_t
checksum(char *s,int n)
{
  uint8_t rval=0;
  for ( int i = 0; i < n; ++i )
    rval += s[i];
  return rval;
}


int pos = 0;
char buf[100];
int count = 1;
long accum = 0;
long errs = 0;
bool debug = false;
// main loop - wait for flag set in interrupt routine
void loop (void)
{
  while (1)
  { 
    noInterrupts();
    bool test = queue.isEmpty();
    interrupts();
    noInterrupts();
    bool full = queue.isFull();
    interrupts();
    if ( full )
      Serial.println("FULL");
    if (test )
      break;
    noInterrupts();
    char t = queue.dequeue();
    interrupts();
    if ( t == '\n' )
    {
      buf[pos] = 0;
      uint8_t cs = checksum(buf,8);
      uint8_t fcs = strtol(&buf[8],NULL,16);
      pos = 0;
      
      if ( debug ) 
      {
        Serial.println(buf);
        Serial.print(fcs,HEX);
        Serial.println(cs,HEX);
      }
      ++count;
      ++accum;
      if ( count == 100 ) {
        Serial.println(queue.count(),DEC);
        Serial.print("accum ");
        Serial.print(accum);
        Serial.print(" errs ");
        Serial.println(errs);
        count = 1;
      }
      
      if (cs != fcs) {
        ++errs;
        Serial.println("checksum error");
        Serial.println(queue.count(),DEC);
        Serial.println(accum,DEC);
        Serial.println(buf);
        Serial.print(fcs,HEX);
        Serial.println(cs,HEX);
        Serial.println("");
      }
    }
    else
      buf[pos++] = t;
  }
    
}  // end of loop
