// include the SoftwareSerial library so you can use its functions:
#include <arduino.h>
#include "SoftTimer.h"
#include "WavTrigger.h"
#define rxPin 8
#define txPin 9

WavTrigger wavTrigger(rxPin,txPin);

int msgCount;
WTInfo info;
void msgCallback(Task* task)
{
  if (wavTrigger.getInfo(&info))
  {
    Serial.println();
    Serial.println(info.version);
    Serial.print("tracks: ");
    Serial.println(info.numTracks,DEC);
  }
  if ( ++msgCount == 200 )
  {
    msgCount = 0;
    wavTrigger.requestInfo();
  }
}

Task msgTimer(10,msgCallback);

void setup()  {
  // define pin modes for tx, rx:
  Serial.begin(9600);
  wavTrigger.begin(57600);
  msgCount = 0;
  SoftTimer.add(&msgTimer);
  memset(&info,0,sizeof(info));
}




