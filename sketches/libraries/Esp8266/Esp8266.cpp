
#include <arduino.h>
#include "Esp8266.h"


Esp8266::Esp8266(uint8_t rx, uint8_t tx, const char *ssid_, const char *password_, int port_)
  : Task(1,Esp8266::msgTaskCallback)
  , espSerial(rx,tx)
  , ssid(ssid_)
  , password(password_)
  , port(port_)
  , cp(buff)
  , msgReady(false)
  , senderId(-1)
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
  state = Reset;
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
  state = WaitReady;
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
  Serial.print("Parsing");
  Serial.println(buff);
  for(cp=buff; *cp != LF; ++cp)
  {
    switch ( state )
    {
      case WaitReady:
        if ( !strncmp(cp,"ready", strlen("ready")))
        {
          Serial.println("got ready");
          clearBuff();
          state = saveState;
          return;
        }
        break;
      case WaitOk:
        if ( !strncmp(cp,"OK", strlen("OK")) )
        {
          Serial.println("got OK");
          clearBuff();
          state = saveState;
          return;
        }
        
      default:
        break;
    }
    if ( !strncmp(cp,"+IPD,", strlen("+IPD,") ) )
    {
      shift();
      senderId = atoi(cp);
      shift();
      int len = atoi(cp);
      shift(':');
      memset(msg,0,sizeof(msg));
      for ( int i = 0; i < len; ++i )
        msg[i] = *cp++;
      msgReady = true;
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
      parseMsg();
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
      
    case Reset:
      sendReset();
      state = EchoOff;
      break;
      
    case EchoOff:
      echoOff();
      state = SetMode;
      break;
      
    case SetMode:
      setMode();
      state = JoinAp;
      break;
      
    case JoinAp:
      joinAp();
      state = SetMux;
      break;
      
    case SetMux:
      setMux();
      state = StartServer;
      break;
      
    case StartServer:
      startServer();
      state = Idle;
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
  espSerial.print("ATE0");
  sendCRLF();
}
void
Esp8266::setMode()
{
  espSerial.print("AT+CWMODE=3");
  sendCRLF();
}
void
Esp8266::joinAp()
{
  espSerial.print("AT+CWJAP=");
  espSerial.print("\"");
  espSerial.print(ssid);
  espSerial.print("\",");
  espSerial.print(password);
  espSerial.print("\"");
  sendCRLF();
}
void
Esp8266::setMux()
{
  espSerial.print("AT+CIPMUX=1");
  sendCRLF();
}
void
Esp8266::startServer()
{
  espSerial.print("AT+CIPSERVER=1,");
  espSerial.print(port,DEC);
  sendCRLF();
}