#include <arduino.h>
#include "Tweeter.h"

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


void
msgCallback(Task *)
{
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
}