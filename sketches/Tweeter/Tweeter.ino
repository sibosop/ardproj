// include the SoftwareSerial library so you can use its functions:
#include <arduino.h>
#include "SoftTimer.h"
#include "WavTrigger.h"
 

WavTrigger wavTrigger;

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
bool gotInfo;

int msgCount;
int infoCount;
WTInfo info;
WTTrackList trackList;
int lastnum = -1;
bool sendTrack;
uint16_t waitTimeout;
uint16_t track;
int repCount;
int listCount;
#define WAIT_INTERVAL 5
#define LIST_REPS 4

void nextTrack()
{
  track = atoi(lp);
  if ( !--repCount )
  {
    repCount = random(1,4);
     
    while( *lp  )
    {
      if ( *lp == ' ')
        break;
        
      ++lp;
    }
    if ( *lp )
      ++lp;
      
    if ( *lp == 0 )
    {
      if ( ++listCount == LIST_REPS)
      {
        listCount = 0;
        if ( ++li == NUM_LISTS )
          li = 0;
      }
      lp = lists[li];
      Serial.print("li:");
      Serial.println(li,DEC);
    }
  }
  track = atoi(lp);
  Serial.print("track:");
  Serial.println(track,DEC);
}


void setNewTrack()
{
  
  nextTrack();
  
  //Serial.print("new track:");
  //Serial.println(track,DEC);
  if ( track )
  {
    WTControlBlock cb(track);
    wavTrigger.controlTrack(&cb);
    waitTimeout = 10000/WAIT_INTERVAL; // Watchdog in case of possible race condition
  }
  else
  {
    waitTimeout = random(100/WAIT_INTERVAL,5000/WAIT_INTERVAL);
  }
}

void msgCallback(Task* task)
{
  if (wavTrigger.getInfo(&info))
  {
#if 1
    Serial.println();
    Serial.println(info.version);
    Serial.print("tracks: ");
    Serial.println(info.numTracks,DEC);
#endif
  }
  if ( !info.valid )
    return;
    
  if ( wavTrigger.getTrackList(&trackList) )
  {
    if ( lastnum != trackList.numTracks )
    {
      lastnum = trackList.numTracks;
      
      //Serial.print("Tracklist num tracks: ");
      //Serial.print(trackList.numTracks,DEC);
      for ( int i =0; i < trackList.numTracks; ++i )
      {
        //Serial.print(" ");
        //Serial.print(trackList.tracks[i],DEC);
      }
      //Serial.println("");
      if ( !lastnum )
        sendTrack=true;
    }
    
  }
  if ( sendTrack )
  {
    sendTrack = false;
    setNewTrack();
  }
  
  if ( --waitTimeout == 0 )
  {
    sendTrack = false;
    setNewTrack();
  }

  wavTrigger.requestTrackList();
}

Task msgTimer(WAIT_INTERVAL,msgCallback);

void setup()  {
  // define pin modes for tx, rx:
  Serial.begin(9600);
  wavTrigger.begin(57600);
  msgCount = 0;
  infoCount = 0;
  memset(&info,0,sizeof(info));
  lastnum = -1;
  wavTrigger.requestInfo();
  SoftTimer.add(&msgTimer);
  randomSeed(analogRead(0));
  sendTrack = false;
  li = 0;
  lp = lists[li];
  repCount = 1;
  listCount = 0;
  nextTrack();
  delay(2000);
  
}




