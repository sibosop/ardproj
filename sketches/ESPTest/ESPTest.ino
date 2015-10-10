// include the SoftwareSerial library so you can use its functions:
#include <arduino.h>
#include "SoftwareSerial.h"
#include "Esp8266.h"
#include "SoftTimer.h"

#define rxPin 6
#define txPin 7
static const int port = 6666;

static const char *ssid = "sibosop";
static const char *password = "sibosopsiboso";
static const char *address = "192.168.1.138"; 

Esp8266 esp = Esp8266(rxPin,txPin,ssid,password,port,address);
enum EspState {
    WaitReady
    ,WaitConnect
    ,SendRequest
    ,WaitResponse
    ,WaitForRequest
};

static EspState espState;
static int requestTimeout;

const char *msg;
void espCallback(Task *) {
  char buff[200];
  int sender;
  if ( esp.getMsg(buff,sender) )
  {
    Serial.print("got:");
    Serial.println(buff);
    espState = WaitForRequest;
    requestTimeout = 500;
  }
  switch ( espState )
  {
    case WaitReady:
      if ( esp.ready() )
      {
        Serial.println("ready");
        espState = WaitConnect;
        esp.connect();
      }
      break;
    
    case WaitConnect:
      if ( esp.isConnected() )
      {
        Serial.println("connected");
        espState = SendRequest;
      }
      break;
      
    case SendRequest:
      Serial.println("sendRequest");
      esp.sendRequest("tweet");
      espState = WaitResponse;
      break;
      
    case WaitForRequest:
      if ( --requestTimeout == 0 )
        espState = WaitReady;
      break;
      
    default:
      break;
  }
}

Task espTimer(10,espCallback);

void setup()  {
  espState = WaitReady;
  // define pin modes for tx, rx:
  Serial.begin(9600);
  // make sure AT+CIOBAUD=9600 has been done
  esp.begin(9600);
  SoftTimer.add(&espTimer);
}





