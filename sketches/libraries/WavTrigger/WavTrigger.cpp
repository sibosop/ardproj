#include <arduino.h>
#include "WavTrigger.h"

WavTrigger::WavTrigger()
 : serialTimerTask(1,WavTrigger::SerialTimerTask::serialTimerCallback)
{
}

void
WavTrigger::begin(long b)
{  
  serialTimerTask.begin(b);
}

bool
WavTrigger::getInfo(WTInfo* i)
{
  if ( !serialTimerTask.info.valid )
    return false;
  
  memcpy(i,&serialTimerTask.info,sizeof(WTInfo));
  serialTimerTask.info.valid = false;
  return true;
}

bool
WavTrigger::getTrackList(WTTrackList* tl)
{
  if ( !serialTimerTask.trackList.valid )
    return false;
  serialTimerTask.trackList.valid = false;
  memcpy(tl,&serialTimerTask.trackList,sizeof(serialTimerTask.trackList));
  return true;
}

void
WavTrigger::requestTrackList()
{
  if ( !serialTimerTask.trackList.valid )
    serialTimerTask.getTrackList();
}

void
WavTrigger::requestInfo()
{
  if ( !serialTimerTask.info.valid )
  {
    serialTimerTask.getVersion();
    serialTimerTask.getInfo();
  }
}


void
WavTrigger::controlTrack(WTControlBlock* cb)
{
  serialTimerTask.controlTrack(cb->track,cb->code);
}
// set the data rate for the SoftwareSerial port




WavTrigger::SerialTimerTask::SerialTimerTask(unsigned long periodMs
                        , void (*callback)(Task* me)
                        )
  : Task(periodMs,callback)
  , serial()
  , length(0)
  , ip(inBuff)
  , readerState(Som1)
{
  memset(inBuff,0,sizeof(inBuff));
  memset((uint8_t *)&info,0,sizeof(info));
  memset((uint8_t *)&trackList,0,sizeof(trackList));
}

void
WavTrigger::SerialTimerTask::begin(long b)
{
  serial.begin(b);
  SoftTimer.add(this);
}

void
WavTrigger::SerialTimerTask::getVersion()
{
  MsgHeader header(5,GET_VERSION);
  serial.write((const uint8_t *)&header,sizeof(header));
  serial.write(EOM);
}

void
WavTrigger::SerialTimerTask::getInfo()
{
  MsgHeader header(5,GET_INFO);
  serial.write((const uint8_t *)&header,sizeof(header));
  serial.write(EOM);
}
void
WavTrigger::SerialTimerTask::getTrackList()
{
  trackList.valid = false;
  MsgHeader header(5,GET_STATUS);
  serial.write((const uint8_t *)&header,sizeof(header));
  serial.write(EOM);
}
void 
WavTrigger::SerialTimerTask::serialTimerCallback(Task* task) 
{
  ((SerialTimerTask*)task)->serialTimer();
}

void
WavTrigger::SerialTimerTask::controlTrack(uint16_t t, ControlCode c)
{
  MsgHeader header(8,CONTROL_TRACK);
  serial.write((const uint8_t *)&header,sizeof(header));
  serial.write(c);
  serial.write(t & 0xff);
  serial.write(t >> 8);
  serial.write(EOM);
}


void
WavTrigger::SerialTimerTask::parseMsg()
{
  //Serial.println(msg,HEX);
  switch ( msg )
  {
    case VERSION_MSG:
      memset(info.version,0,sizeof(info.version));
      memcpy(info.version,inBuff,length);
      //getInfo();
      break;
      
    case INFO_MSG:
      ip = inBuff;
      ++ip;
      info.numTracks = *((uint16_t *)ip);
      info.valid = true;
      break; 
    
    case STATUS_MSG:
    {
      //Serial.println("Got Status Msg");
      uint16_t *tp = (uint16_t *)&inBuff[0];
      trackList.numTracks = 0;
      for ( int i = 0; i < length/2; ++i)
      {
        trackList.tracks[trackList.numTracks++] = *tp++;
      }
      trackList.valid = true;
    }
    
    default:
    break;
  }
}


void 
WavTrigger::SerialTimerTask::serialTimer()
{
  while ( serial.available() )
  {  
    uint8_t c = serial.read();
    //Serial.print("c "); Serial.println(c,HEX);
    switch ( readerState )
    {
		  case Som1:
		    if ( c != SOM1 )
		    {
          Serial.print("som1 out of sync on ");
          Serial.println(c,HEX);
		    }
		    else
		    {
          readerState = Som2;
		    }
        break;
        
		  case Som2:
		    if ( c != SOM2 )
		    {
          Serial.print("som2 out of sync on ");
          Serial.println(c,HEX);
          readerState = Som1;
		    }
		    else
		    {
          readerState = Length;
		    }
        break;
        
      case Length:
        lcnt = c - 5;
        length = lcnt;
        readerState=Msg;
        //Serial.print("length ");Serial.println(length,DEC);
        break;
      
      case Msg:
        msg=c;
        readerState = Data;
        ip=inBuff;
        break;
      
      case Data:
       // Serial.print("lcnt ");
        //Serial.println(lcnt,DEC);

        if ( lcnt-- == 0 )
        {
          readerState = Eom;
        }
        else
        {
          *ip++ = (char)c;
          //Serial.println(inBuff);
          break;
        }
        // fall through on Eom
      
      case Eom:
        if ( c != EOM )
		    {
          Serial.print("eom out of sync on ");
          Serial.println(c,HEX);
		    }
		    else
		    {
          parseMsg();
          memset(inBuff,0,sizeof(inBuff));
		    }
        readerState = Som1;
        break;
        
      default:
        readerState = Som1;
        break;
    }
	}
}



