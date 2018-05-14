
#include <AltSoftSerial.h>

AltSoftSerial altSerial;

void setup() {
  altSerial.begin(9600);
}

void clear()
{
  altSerial.write(0xfe);
  altSerial.write(0x51);
}

void setRC(int r, int c)
{
  uint8_t val = 0;
  if ( r )
    val = 0x40;
  val += c;
  altSerial.write(0xfe);
  altSerial.write(0x45);
  altSerial.write(val);
}
void loop() {
  clear();
  delay(1000);
#if 1
  //setRC(0,0);
  //delay(1000);
  altSerial.print("01234567890123456");
  delay(1000);
  setRC(1,0);
  delay(1000);
  altSerial.print("01234567890123456");
  delay(1000);
#endif
}
