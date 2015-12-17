#include <arduino.h>
#include <Tweeter.h>
#include <General.h>

#define NUM_LISTS 5

char list[100];
char *lp;

int li;
uint16_t lastVol;
static const int VolCount = 100;
int count;


Tweeter tweeter;
int waitTime;
#define TWEET_WAIT 10
bool
nextList()
{
#if 1
  Serial.print("nextList:");
  if ( !tweeter.getNumTracks() )
  {
    Serial.println("wait for num tracks");
    return false;
  }
  uint8_t num = random(3,6);
  char *cp = list;
  for ( int i = 0; i < num; ++i )
  {
    char buff[8];
    memset(buff,0,sizeof(buff));
    String cn = String(random(tweeter.getNumTracks()+1));
    cn.toCharArray(buff,sizeof(buff)-1);
    strcpy(cp,buff);
    cp += strlen(buff);
    *cp++ = ' ';
    *cp = 0;
  }
  
  lp = list;
  DUMP(lp); 
#endif
  return true;
}



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
      //DUMP(vo);
      tweeter.setVolume(vo);
    }
  }
  if ( tweeter.needsList() )
  {
    if ( nextList() )
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
  //delay(2000);
  lastVol = 0;
  count = 0;
}