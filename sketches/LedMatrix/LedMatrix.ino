
#include <SoftTimer.h>
#include <LedMatrix.h>
#include <Fly.h>
#include <Trap.h>

const int latchPin = 4;
const int resetPin = 5;
const int flyCount = 4;

Fly flies[flyCount];
Trap trap;
const int numPixels = 64;
Pixel pixels[numPixels]; 
void patternCallback(Task* task) {
  LedMatrix.clearDisplay();
#if 0
  for ( int f = 0; f < flyCount; ++f )
    flies[f].move();
  trap.display();
#endif
  for(int i = 0; i < numPixels; ++i )
  {
    //pixels[i].c.pulse();
#if 0
    Serial.print(i);
    Serial.print(" ");
    Serial.print(pixels[i].c.red,HEX);
    Serial.print(" ");
    Serial.print(pixels[i].c.green,HEX);
    Serial.println();
#endif
    LedMatrix.setPixel(pixels[i]);
  }
  Serial.println();
  LedMatrix.display();
}
Task patternTimer(1000,patternCallback);



void setup() {
  //set pins to output because they are addressed in the main loop
  LedMatrix.begin(latchPin,resetPin);
  Serial.begin(9600);
  SoftTimer.add(&patternTimer);
  randomSeed(analogRead(0));
  LedMatrix.drawEnable = true;
  pixels[0]= Pixel(Pos(0,0),Color(1,0));
  pixels[1]=Pixel(Pos(0,1),Color(2,0));
  pixels[2]=Pixel(Pos(0,2),Color(3,0));
  pixels[3]=Pixel(Pos(0,3),Color(4,0));
  pixels[4]= Pixel(Pos(0,4),Color(5,0));
  pixels[5]=Pixel(Pos(0,5),Color(6,0));
  pixels[6]=Pixel(Pos(0,6),Color(7,0));
  pixels[7]=Pixel(Pos(0,7),Color(8,0));
  
  pixels[8]= Pixel(Pos(1,0),Color(9,0));
  pixels[9]=Pixel(Pos(1,1),Color(10,0));
  pixels[10]=Pixel(Pos(1,2),Color(11,0));
  pixels[11]=Pixel(Pos(1,3),Color(12,0));
  pixels[12]= Pixel(Pos(1,4),Color(13,0));
  pixels[13]=Pixel(Pos(1,5),Color(14,0));
  pixels[14]=Pixel(Pos(1,6),Color(15,0));
  pixels[15]=Pixel(Pos(1,7),Color(16,0));
  
  pixels[16]= Pixel(Pos(2,0),Color(0,1));
  pixels[17]=Pixel(Pos(2,1),Color(0,3));
  pixels[18]=Pixel(Pos(2,2),Color(0,5));
  pixels[19]=Pixel(Pos(2,3),Color(0,7));
  pixels[20]= Pixel(Pos(2,4),Color(0,9));
  pixels[21]=Pixel(Pos(2,5),Color(0,11));
  pixels[22]=Pixel(Pos(2,6),Color(0,13));
  pixels[23]=Pixel(Pos(2,7),Color(0,15));
  
  pixels[24]= Pixel(Pos(3,0),Color(0,17));
  pixels[25]=Pixel(Pos(3,1),Color(0,19));
  pixels[26]=Pixel(Pos(3,2),Color(0,21));
  pixels[27]=Pixel(Pos(3,3),Color(0,23));
  pixels[28]= Pixel(Pos(3,4),Color(0,25));
  pixels[29]=Pixel(Pos(3,5),Color(0,27));
  pixels[30]=Pixel(Pos(3,6),Color(0,29));
  pixels[31]=Pixel(Pos(3,7),Color(0,31));
  
  pixels[32]= Pixel(Pos(4,0),Color(1,1));
  pixels[33]=Pixel(Pos(4,1),Color(2,2));
  pixels[34]=Pixel(Pos(4,2),Color(3,3));
  pixels[35]=Pixel(Pos(4,3),Color(4,4));
  pixels[36]= Pixel(Pos(4,4),Color(5,5));
  pixels[37]=Pixel(Pos(4,5),Color(6,6));
  pixels[38]=Pixel(Pos(4,6),Color(7,7));
  pixels[39]=Pixel(Pos(4,7),Color(8,8));
  
  pixels[40]= Pixel(Pos(5,0),Color(9,9));
  pixels[41]=Pixel(Pos(5,1),Color(10,10));
  pixels[42]=Pixel(Pos(5,2),Color(11,11));
  pixels[43]=Pixel(Pos(5,3),Color(12,12));
  pixels[44]= Pixel(Pos(5,4),Color(13,13));
  pixels[45]=Pixel(Pos(5,5),Color(14,14));
  pixels[46]=Pixel(Pos(5,6),Color(15,15));
  pixels[47]=Pixel(Pos(5,7),Color(16,16));
  
  pixels[48]= Pixel(Pos(6,0),Color(1,2));
  pixels[49]= Pixel(Pos(6,1),Color(2,4));
  pixels[50]=Pixel(Pos(6,2),Color(3,6));
  pixels[51]=Pixel(Pos(6,3),Color(4,8));
  pixels[52]=Pixel(Pos(6,4),Color(5,10));
  pixels[53]=Pixel(Pos(6,5),Color(6,12));
  pixels[54]=Pixel(Pos(6,6),Color(7,14));
  pixels[55]=Pixel(Pos(6,7),Color(8,16));
  
  pixels[56]= Pixel(Pos(7,0),Color(9,18));
  pixels[57]= Pixel(Pos(7,1),Color(10,20));
  pixels[58]=Pixel(Pos(7,2),Color(11,22));
  pixels[59]=Pixel(Pos(7,3),Color(12,24));
  pixels[60]=Pixel(Pos(7,4),Color(13,26));
  pixels[61]=Pixel(Pos(7,5),Color(14,28));
  pixels[62]=Pixel(Pos(7,6),Color(15,30));
  pixels[63]=Pixel(Pos(7,7),Color(16,31));
  
  
  
  
  
  
  
  
  
}
