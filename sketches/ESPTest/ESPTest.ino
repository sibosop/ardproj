// include the SoftwareSerial library so you can use its functions:
#include <arduino.h>
#include "SoftwareSerial.h"
#include "Esp8266.h"
#include "SoftTimer.h"

#define rxPin 6
#define txPin 7
#define port  23

static const char *ssid = "sibosop";
static const char *password = "sibosopsiboso";

Esp8266 esp = Esp8266(rxPin,txPin,ssid,password,port);

const char *msg;
void espCallback(Task *) {
  char buff[200];
  int sender;
  if ( esp.getMsg(buff,sender) )
  {
    Serial.print("got:");
    Serial.print(buff);
    Serial.print(" from sender ");
    Serial.println(sender,DEC);
  }
}

Task espTimer(10,espCallback);

void setup()  {
  // define pin modes for tx, rx:
  Serial.begin(9600);
  esp.begin(115200);
  SoftTimer.add(&espTimer);
}





