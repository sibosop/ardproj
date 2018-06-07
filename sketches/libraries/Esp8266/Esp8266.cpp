
#include <arduino.h>
#include "Esp8266.h"


Esp8266::Esp8266(uint8_t rx, uint8_t tx, const char *ssid_
          , const char *password_, int port_, const char *address_)
  : Task(1,Esp8266::msgTaskCallback)
  , espSerial(rx,tx)
  , ssid(ssid_)
  , password(password_)
  , address(address_)
  , port(port_)
  , cp(buff)
  , msgReady(false)
  , senderId(-1)
  , initDone(false)
  , connected(false)
{
}

void 
Esp8266::begin(long baud)
{
  espSerial.begin(baud);
  SoftTimer.add(this);
  reset();
}

void
Esp8266::clearBuff()
{
  memset(buff,0,sizeof(buff));
  cp = buff;
}

void
Esp8266::reset()
{
  clearBuff();
  state = WaitReset;
  initDone=false;
  waitTimer = 5000;
}

#define CR  0xD
#define LF  0xA
void
Esp8266::sendCRLF()
{
  espSerial.write(CR);
  espSerial.write(LF);
  saveState = state;
  state = WaitOk;
}
void
Esp8266::sendReset()
{
  espSerial.print("AT+RST");
  sendCRLF();
  state=WaitReady;
}



bool 
Esp8266::getMsg(char *ret, int& id)
{
  if (msgReady)
  {
    msgReady = false;
    id = senderId;
    strcpy(ret,msg);
    return true;
  }
  return false;
}

void
Esp8266::shift(char t)
{
  while (*cp && (*cp != t))
    ++cp;
  if ( *cp == t )
    ++cp;
}

void
Esp8266::parseMsg()
{
  Serial.print("Parsing:");
  Serial.println(buff);

    switch ( state )
  {
    case WaitConnect:
      if ( !strncmp(buff,"ERROR",strlen("ERROR")))
      {
        Serial.println("Got ERROR during connect");
        state = WaitConnect;
        return;
      }
      if ( !strncmp(buff,"CLOSED",strlen("CLOSED")))
      {
        Serial.println("Got CLOSED during connect retrying...");
        connected = false;
        state = RetryConnect;
        waitTimer = 5000;
        return;
      }
      if ( !strncmp(buff,"CONNECT", strlen("CONNECT")))
      {
        Serial.println("got CONNECT");
        
        state = saveState;
        connected = true;
        return;
      }
    case WaitReady:
      if ( !strncmp(buff,"ready", strlen("ready")))
      {
        Serial.println("got ready");
        
        state = saveState;
        return;
      }
      break;
    case WaitOk:
      if ( !strncmp(buff,"OK", strlen("OK")) )
      {
        Serial.println("got OK");
        
        state = saveState;
        return;
      }
      break;
    
    case Idle:
      break;
      
    case RetryConnect:
      state = Idle;
      break;
      
    default:
      Serial.print("BAD STATE:");
      Serial.println(state,DEC);
      break;
  }
  if ( !strncmp(buff,"ERROR",strlen("ERROR")))
  {
    Serial.println("Got ERROR");
    state = Error;
  }
  if ( !strncmp(buff,"CLOSED",strlen("CLOSED")))
  {
    Serial.println("Got CLOSED");
    connected = false;
    state = Idle;
  }
  
  if ( !strncmp(buff,"+IPD,", strlen("+IPD,") ) )
  {
    Serial.println("Got +IPD");
    cp = buff;
    Serial.println(cp);
    shift();
    Serial.println(cp);
    int len = atoi(cp);
    Serial.print("length:");
    Serial.println(len,DEC);
    shift(':');
    Serial.println(cp);
    
    memset(msg,0,sizeof(msg));
    for ( int i = 0; i < len; ++i )
      msg[i] = *cp++;
    Serial.print("msg:");
    Serial.println(msg);
    msgReady = true;
    if ( !strncmp(cp,"CLOSED",strlen("CLOSED")))
    {
      Serial.println("Got CLOSED");
      connected = false;
      state = Idle;
    }
  }    
}


void
Esp8266::msgTask()
{
  if ( espSerial.available() )
  {
    *cp = espSerial.read();
    if ( *cp == LF )
    {
      parseMsg();
      clearBuff();
    }
    else
      ++cp;
  }
  switch ( state )
  {
    default:
      break;
    
    case Error:
      Serial.println("Got Error");
      state = Reset;
      break;
      
    case WaitReset:
      if (--waitTimer == 0 )
      {
        Serial.println("starting reset sequence");
        state = Reset;
      }
      break;
      
    case Reset:
      Serial.println("sending reset sequence");
      state = EchoOff;
      sendReset();
      
      break;
      
    case EchoOff:
      state = SetMode;
      echoOff();
      
      break;
      
    case SetMode:
      state = JoinAp;
      setMode();
      break;
      
    case JoinAp:
      state = SetMux;
      joinAp();
      break;
      
    case SetMux:
      state = InitDone;
      setMux();
      break;
      
    case StartServer:
      state = Idle;
      startServer();
      break;
      
    case InitDone:
      Serial.println("Init done");
      initDone = true;
      state = Idle;
      break;
      
    case WaitForInput:
      espSerial.print(request);
      state = Idle;
      sendCRLF();
      break;
      
    case RetryConnect:
      if (--waitTimer==0)
        connect();
      break;
   
  }
}

void
Esp8266::msgTaskCallback(Task *t)
{
  ((Esp8266 *)t)->msgTask();
}


void
Esp8266::echoOff()
{
  Serial.println("echo off");
  espSerial.print("ATE0");
  sendCRLF();
}
void
Esp8266::setMode()
{
  Serial.println("set mode");
  espSerial.print("AT+CWMODE=3");
  sendCRLF();
}
void
Esp8266::joinAp()
{
  Serial.println("join ap");
  espSerial.print("AT+CWJAP=");
  espSerial.print("\"");
  espSerial.print(ssid);
  espSerial.print("\",");
  espSerial.print("\"");
  espSerial.print(password);
  espSerial.print("\"");
  sendCRLF();
}
void
Esp8266::setMux()
{
  Serial.println("setMux");
  espSerial.print("AT+CIPMUX=0");
  sendCRLF();
}
void
Esp8266::startServer()
{
  Serial.println("start server");
  espSerial.print("AT+CIPSERVER=1,");
  espSerial.print(port,DEC);
  sendCRLF();
}

 
void
Esp8266::connect()
{
  if ( connected )
    return;
  Serial.println("connect to server");
  Serial.print("AT+CIPSTART=\"TCP\",");
  Serial.print("\"");
  Serial.print(address);
  Serial.print("\",");
  Serial.println(port,DEC);
  espSerial.print("AT+CIPSTART=\"TCP\",");
  espSerial.print("\"");
  espSerial.print(address);
  espSerial.print("\",");
  espSerial.print(port,DEC);
  sendCRLF();
  state=WaitConnect;
}

void
Esp8266::sendRequest(const char *r)
{
  strcpy(request,r);
  Serial.print("AT+CIPSEND=");
  Serial.println(strlen(request)+2,DEC);
  espSerial.print("AT+CIPSEND=");
  espSerial.print(strlen(request)+2,DEC);
  state=WaitForInput;
  sendCRLF();
}
