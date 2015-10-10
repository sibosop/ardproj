// include the SoftwareSerial library so you can use its functions:
#include "Tweeter.h"
 


#define LIST_REPS 4

void 
Tweeter::nextTrack()
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
      return;
    }
  }
  track = atoi(lp);
  Serial.print("track:");
  Serial.println(track,DEC);
}


void 
Tweeter::setNewTrack()
{
  if ( needsList() )
    return;
    
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

void 
Tweeter::check()
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
  if ( needsList() )
    return;
    
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

void
Tweeter::msgCallback(Task *t)
{
  ((Tweeter *)t)->check();
}

Tweeter::Tweeter() 
  : Task(WAIT_INTERVAL,msgCallback)
  , msgCount(0)
  , lastnum(-1)
  , sendTrack(false)
  , repCount(1)
  , lp(list)
{
  memset(&info,0,sizeof(info));
  memset(list,0,sizeof(list));
  memset(newList,0,sizeof(newList));
}

void 
Tweeter::begin()
{
  wavTrigger.begin(57600);
  SoftTimer.add(this);
  wavTrigger.requestInfo();
  randomSeed(analogRead(0));
  nextTrack();  
}

void
Tweeter::setList(const char *l)
{
  strcpy(list,l);
  lp = list;
  nextTrack();
}

bool
Tweeter::needsList()
{
  return *lp == 0;
}
