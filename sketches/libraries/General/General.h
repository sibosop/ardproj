#ifndef GENERAL_H
#define GENERAL_H
#include <arduino.h>

#define DUMP(a) \
do { \
  Serial.print(#a);\
  Serial.print(":");\
  Serial.print(a);\
  Serial.println(" ");\
  }while(0)

#define HDUMP(a) \
do{\
  Serial.print(#a);\
  Serial.print(":");\
  Serial.print(a,HEX);\
  Serial.println(" ");\
  }while(0)
#endif
		
inline int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
