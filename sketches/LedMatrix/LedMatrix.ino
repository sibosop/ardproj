
#include <SoftTimer.h>
#include <LedMatrix.h>

const int latchPin = 4;
const int resetPin = 5;
uint8_t coff = 0;
uint8_t boff = 0;
bool up;


void fillRect(Pos ul, Pos lr, Color col)
{
  //Serial.println("enter");
  for ( int r = ul.row; r < lr.row; ++r )
  {
    for ( int c = ul.col; c < lr.col; ++c )
    {
      Pixel p(Pos(r,c),col);
      LedMatrix.setPixel(p);
    }
  }
  //Serial.println("exit");
}



void patternCallback(Task* task) {
  LedMatrix.clearDisplay();
#if 1
  if ( up )
  {
    if (boff == 15)
      up = false;
    else
      ++boff;
  } 
  else
  {
    if ( boff == 0 )
    {
      ++coff;
      up = true;
    }
    else
      --boff;
  }
  
#endif
  
  
  Serial.println(boff);
  fillRect(Pos(0,0),Pos(4,4),Color((Color::Colors)((coff+3) % 4),boff));
  fillRect(Pos(0,4),Pos(4,8),Color((Color::Colors)((coff+2) % 4),boff));
  fillRect(Pos(4,4),Pos(8,8),Color((Color::Colors)((coff+1) % 4),boff));
  fillRect(Pos(4,0),Pos(8,4),Color((Color::Colors)((coff+0) % 4),boff));
  
  
#if 0
  int color = coff;
  int bright = boff;
  for ( int r = 0; r < 8; r++ )
  {
    for (int c = 0; c < 8; c++ )
    {
      Pixel p(Pos(r,c),Color((Color::Colors)color,bright));
      LedMatrix.setPixel(p);
      if ( ++bright == 16 )
      {
        bright = 0;
        if (++color == Color::MaxColors)
          color = 0;
      }
    }
  }
  if ( !boff )
  {
    coff = (--coff % 4);
    boff = 8;
  } 
  else
  { 
    boff = 0;
  }
#endif

  LedMatrix.display();
}
Task patternTimer(30,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  LedMatrix.drawEnable = true;
  boff = 0;
  coff = 0;
  up = true;
}
