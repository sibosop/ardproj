#include <RgbScreenBuffer.h>

RgbScreenBuffer::RgbScreenBuffer(uint8_t dataPin, uint8_t clockPin)
  : strip(rows*cols, dataPin, clockPin)
{}