#include <SPI.h>


#define MISO 12
#define MOSI 11
#define SPICLK 13

const int dataPin  = MOSI;
const int clockPin = SPICLK;
const int latchPin = 4;
const int resetPin = 5;


void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(MOSI, OUTPUT); 
  pinMode(SPICLK, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(MISO,INPUT);
  Serial.begin(9600);
  Serial.println("reset");
  digitalWrite(latchPin, LOW);
  digitalWrite(resetPin,0);
  delay(10);
  digitalWrite(resetPin,1);
  digitalWrite(latchPin, HIGH);
  SPI.begin();
}

struct RowVal {
  byte vals[3];
  
  RowVal() 
  {
    memset(vals,0,3);
  }
    
  void set(byte r, byte gp, byte rp)
  {
    memset(vals,0,3);
    bitWrite(vals[2],r,1);
    bitWrite(vals[1],gp,1);
    bitWrite(vals[0],rp,1);
  }
  void print()
  {
    Serial.print("row:"); Serial.print(vals[2]);
    Serial.print(" green: "); Serial.print(vals[1]);
    Serial.print(" red: "); Serial.println(vals[0]);
  }
};

void loop() {
  
  for ( int i = 0; i < 8; i++ )
  {
    
    RowVal rv;
    

    // turn off the output so the pins don't light up
    // while you're shifting bits
    // turn on the next highest bit in bitsToSend:
    for ( int j = 0; j < 8; ++j )
    {
      rv.set(i,j,7-j);
      //rv.print();
      digitalWrite(latchPin, LOW);
      //digitalWrite(clockPin, LOW);
      SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
      SPI.transfer(rv.vals,3);
      SPI.endTransaction();
#if 0
      shiftOut(dataPin, clockPin, MSBFIRST, rv.greenCol);
      shiftOut(dataPin, clockPin, MSBFIRST, rv.redCol);
      shiftOut(dataPin, clockPin, MSBFIRST, rv.row);
#endif
      digitalWrite(latchPin, HIGH);
      delay(50);
    }
    
  }
}