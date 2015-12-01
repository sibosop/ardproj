#ifndef GENERAL_H
#define GENERAL_H

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