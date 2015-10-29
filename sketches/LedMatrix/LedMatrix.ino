



const int dataPin  = 2;
const int clockPin = 3;
const int latchPin = 4;
const int resetPin = 5;


void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  pinMode(clockPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
  digitalWrite(latchPin, LOW);
  digitalWrite(resetPin,0);
  delay(10);
  digitalWrite(resetPin,1);
  digitalWrite(latchPin, HIGH);
}

void loop() {
  
  for ( int i = 0; i < 8; i++ )
  {
    
    byte rows = 0;

    // turn off the output so the pins don't light up
    // while you're shifting bits:
    

    // turn on the next highest bit in bitsToSend:
    
    
    bitWrite(rows, i, 1);
    for ( int j = 0; j < 8; ++j )
    {
      byte color = 0;
      bitWrite(color,j,1);
      digitalWrite(latchPin, LOW);
      digitalWrite(clockPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, color);
      shiftOut(dataPin, clockPin, MSBFIRST, color);
      shiftOut(dataPin, clockPin, MSBFIRST, rows);
      digitalWrite(latchPin, HIGH);
      delay(50);
    }
    
  }
}