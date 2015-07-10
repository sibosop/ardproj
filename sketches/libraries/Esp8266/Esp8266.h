#ifndef ESP8266_H
#define ESP8266_H
#include <stdint.h>
#include "SoftwareSerial.h"
#include "SoftTimer.h"

class Esp8266 : public Task {
public:
  Esp8266(uint8_t rx, uint8_t tx, const char *ssid_, const char *password_, int port_);
  bool getMsg(char *,int&);
  void begin(long baud);
private:
  void clearBuff();
  static void msgTaskCallback(Task *);
  void reset();
  SoftwareSerial espSerial;
  String  ssid;
  String  password;
  int port;
  char buff[200];
  char msg[200];
  char *cp;
  bool msgReady;
  int senderId;
  enum State 
  {
    Error
    ,Reset
    ,EchoOff
    ,SetMode
    ,JoinAp
    ,SetMux
    ,StartServer
    ,WaitOk
    ,WaitReady
    ,Idle
  };
  State state;
  State saveState;
  void sendReset();
  void echoOff();
  void setMode();
  void joinAp();
  void setMux();
  void startServer();
  void shift(char delim=',');
  void sendCRLF();
  void parseMsg();
  void msgTask();
};
#endif