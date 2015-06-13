#ifndef STRIPPER_H
#define STRIPPER_H
#include <arduino.h>
class Stripper {
public:
	virtual void show() = 0;
	virtual void setPixelColor(uint16_t n, uint32_t c) = 0;
	virtual void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) = 0;
};
#endif