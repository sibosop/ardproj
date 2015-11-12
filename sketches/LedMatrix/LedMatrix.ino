
#include <SoftTimer.h>
#include <LedMatrix.h>
#include <Fly.h>
#include <Trap.h>

const int latchPin = 4;
const int resetPin = 5;
const int flyCount = 4;

Fly flies[flyCount];
Trap trap;
const int numPixels = 64;
Pixel pixels[numPixels]; 
void patternCallback(Task* task) {
  LedMatrix.clearDisplay();
  int color = 0;
  int bright = 0;
  for ( int r = 0; r < 8; r++ )
  {
    for (int c = 0; c < 8; c++ )
    {
      Pixel p(Pos(r,c),Color::palette[color][bright]);
      LedMatrix.setPixel(p);
      if ( ++bright == 16 )
      {
        bright = 0;
        if (++color == Color::MaxColors)
          color = 0;
      }
    }
  }
  LedMatrix.display();
}
Task patternTimer(1000,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  LedMatrix.drawEnable = true;
  
  
  
  
  
  
  
  
  
}
