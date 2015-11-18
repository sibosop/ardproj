
#include <SoftTimer.h>
#include <LedMatrix.h>

const int latchPin = 4;
const int resetPin = 5;
uint8_t coff = 0;
uint8_t val = 0;
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

void
putNumber(int num,Color c,int chaos)
{
  int ten = num/10;
  int one = num % 10;
  LedMatrix.drawPosMap(number[ten],c,1,chaos);
  LedMatrix.drawPosMap(number[one],c,5,chaos);
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
        val = random(100);
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
  putNumber(val,Color((Color::Colors)(3-clr),15),coff);
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
  wcnt = 0;
  val = 0;
  clr = 0;
}
