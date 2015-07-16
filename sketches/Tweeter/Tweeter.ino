// include the SoftwareSerial library so you can use its functions:
#include <arduino.h>
#include "SoftTimer.h"
#include "WavTrigger.h"
 

WavTrigger wavTrigger;

bool gotInfo;

int msgCount;
int infoCount;
WTInfo info;
WTTrackList trackList;
int lastnum = -1;
bool sendTrack;
uint16_t waitTimeout;
#define WAIT_INTERVAL 5

void setNewTrack()
{
  uint16_t track = 0;
  if (info.valid)
  {
    track = random(0,info.numTracks+1);
  }
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
#if 0
    Serial.println();
    Serial.println(info.version);
    Serial.print("tracks: ");
    Serial.println(info.numTracks,DEC);
#endif
    gotInfo = true;
  }
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

  if ( gotInfo )
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
  gotInfo = false;
  lastnum = -1;
  wavTrigger.requestInfo();
  SoftTimer.add(&msgTimer);
  delay(2000);
  randomSeed(analogRead(0));
  sendTrack = false;
}




