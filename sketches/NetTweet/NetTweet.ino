#include <arduino.h>
#include <Tweeter.h>
#include <General.h>

#define NUM_LISTS 5
const char * const lists[NUM_LISTS] =
{
  "1 2 3 0 4"
  ,"5 6 7 8 9 10"
  ,"11 12 13 14 15"
  ,"16 17 18 19"
  ,"20 0 11 4 15"
};

int li;
const char *lp;

Tweeter tweeter;
int waitTime;
#define TWEET_WAIT 10
void
nextList()
{
  Serial.print("nextList:");
  Serial.println(li,DEC);
  lp = lists[li];
  
  if (++li == NUM_LISTS)
    li = 0;
}


uint16_t lastVol;
static const int VolCount = 100;
int count;
void
msgCallback(Task *)
{
  if ( ++count == VolCount )
  {
    count = 0;
    uint16_t v = analogRead(A1);
    v >>= 3;
    float tmp = v;
    tmp /=  (128.0 / 80.0);
    v = tmp;
    //DUMP(v);
    //DUMP(tmp);
    if(lastVol != v)
    { 
      lastVol = v;
      int16_t vo = -70 + v;
      DUMP(vo);
      tweeter.setVolume(vo);
    }
  }
  if ( tweeter.needsList() )
  {
    nextList();
    tweeter.setList(lp);
  }
}

Task msg(10,msgCallback);

void
setup()
{
  Serial.begin(9600);
  nextList();
  li = 0;
  tweeter.begin();
  SoftTimer.add(&msg);
  delay(2000);
  lastVol = 0;
  count = 0;
}