#include <RgbScreenBuffer.h>
#include <Numbers.h>

RgbScreenBuffer::RgbScreenBuffer(uint8_t dataPin, uint8_t clockPin)
  : strip(rows*cols, dataPin, clockPin)
{}


void
RgbScreenBuffer::putNumber(uint8_t o, uint8_t n, const RgbPixel& px)
{
  const Pos *p; 
  for(p = number[n];p->row != 0xFF;++p)
  {
    Pos t(*p);
    t.col += o;
    t.row -= 1;
    //t.dump();
    setPixel(t,px);
  }
}