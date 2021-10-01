#ifndef RGBRamp_h
#define RGBRamp_h
#include <stdint.h>
#include <arduino.h>


class RGBRamp {
public:
	RGBRamp(uint16_t max=256);

	void	step();
	uint32_t  getVal(void);
	bool changed() { return changed_; }
	
  private:
	uint8_t	curVal[3];
  uint16_t maxDest;
	uint8_t	dest[3];
	uint16_t curSpeed[3];
	uint16_t	speed[3];
	bool	stopped[3];
	bool changed_;
	enum SpeedLimits {
		LowSpeed = 10
		,HighSpeed = 30
		,StoppedSpeed = 1000
	};
};

#endif
