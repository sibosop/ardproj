#ifndef FLEA_H
#define FLEA_H
class Flea {
public:
	uint8_t pos;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t getLane() {
		return pos & 0x3;
	}
	uint8_t getPos() {
		return pos >> 2;
	}
};
#endif