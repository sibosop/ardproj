
#include <SoftTimer.h>
#include <LedMatrix.h>

const int latchPin = 4;
const int resetPin = 5;
uint8_t coff = 0;
uint8_t boff1 = 0;
uint8_t boff2 = 0;
uint8_t wcnt = 0;
uint8_t clr = 0;
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
  if ( !wcnt )
  {
    if (up)
    {
      if (++coff == 7)
      {
        up = false;
        clr = random(Color::MaxColors);
      }
    } 
    else
    {
      if ( --coff == 0 )
      {
        up = true;
        boff1 = random(10);
        boff2 = random(10); 
        wcnt = 50;
        
      }
    }
  }
  else
  {
    --wcnt;
  }

  //Serial.println(coff);
  fillRect(Pos(0,0),Pos(8,8),Color((Color::Colors)clr,coff));
  LedMatrix.drawPosMap(number[boff1],Color((Color::Colors)(3-clr),15),1,coff);
  LedMatrix.drawPosMap(number[boff2],Color((Color::Colors)(3-clr),15),5,coff);
  LedMatrix.display();
}
Task patternTimer(50,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  LedMatrix.drawEnable = true;
  coff = 0;
  up = true;
}
