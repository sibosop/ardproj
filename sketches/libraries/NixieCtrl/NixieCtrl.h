#ifndef NIXIE_CTRL
#define NIXIE_CTRL

class NixieCtrl
{
private:
	static const int numTubes = 4;
	static const int clk = 13;
	static const int mosi = 11;
public:
	void begin(int latchPin_,int clearPin_)
	{
		latchPin = latchPin;
		clearPin = clearPin;
		
	}
private:
	int latchPin;
	int clearPin;
			
	
	
};

#endif