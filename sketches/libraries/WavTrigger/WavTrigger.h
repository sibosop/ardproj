#ifndef WaveTrigger_H
#define WaveTrigger_h
#include <AltSoftSerial.h>
#include <SoftTimer.h>
#define SOM1  0xF0
#define SOM2  0xAA
#define EOM   0x55
#define GET_VERSION 0x1
#define GET_INFO 0x2
#define GET_STATUS 0x7
#define VERSION_MSG 0x81
#define INFO_MSG 0x82
#define STATUS_MSG 0x83
#define MaxTracks 14
enum ControlCode {
  PlaySolo = 0
  ,PlayPoly
  ,Pause
  ,Resume
  ,LoopOn
  ,LoopOff
  ,Load
};

struct WTTrackList
{
  bool valid;
  int numTracks;
  uint16_t tracks[MaxTracks];
  WTTrackList()
    : valid(false)
    , numTracks(0)
    {}
};

struct WTControlBlock
{
  uint16_t track;
  ControlCode code;
  int16_t volume;
  int16_t fadeVolume;
  uint16_t fadeSpeed;
  uint8_t fadeStop;  
};

struct WTInfo
{
  bool valid;
  char version[21];
  uint16_t numTracks;
  WTInfo()
    : valid(false)
    {}
};

class WavTrigger 
{
public:
  typedef char VersionString[20];
  WavTrigger();
  void begin(long);
  bool getTrackList(WTTrackList*);
  bool getInfo(WTInfo*);
  void requestTrackList();
  void requestInfo();
  void controlTrack(WTControlBlock*);
  void stopAll();
  void resumeAllSync();
  void volume(int16_t);
  
private:
  class SerialTimerTask : public Task
  {
    public: 
      friend WavTrigger;
      
      SerialTimerTask(unsigned long periodMs, void (*callback)(Task* me));
      
      
    private:
      AltSoftSerial serial;
      uint8_t length;
      uint8_t lcnt; 
      char *ip;
      char inBuff[256];
      
      WTInfo info;
      WTTrackList trackList;
      uint8_t msg;
      
      struct MsgHeader
      {
        uint8_t som1;
        uint8_t som2;
        uint8_t length;
        uint8_t msg;
        MsgHeader(uint8_t l, uint8_t m)
          : som1(SOM1)
          , som2(SOM2)
          , length(l)
          , msg(m)
          {}
      };
      
      void parseMsg();
      enum ReaderState {
        Som1
        ,Som2
        ,Length
        ,Msg
        ,Data
        ,Eom
      };
      ReaderState readerState;
      
      
      static void serialTimerCallback(Task *);
      void serialTimer(); 
      void begin(long b);
      void getInfo();
      void getVersion();
      void getTrackList();
      
  };
  SerialTimerTask serialTimerTask;
  
    
};

#endif