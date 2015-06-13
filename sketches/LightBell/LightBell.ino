
//#include "SoftTimer.h"
#include "arduino.h" 

#if 0
#define TOTAL (((4*60) + 33) * 2)
//#define TOTAL 10


class LedTask : public Task
{
public:
    LedTask(unsigned long periodMs, void (*callback)(Task* me))  
    : Task(periodMs,callback)
    , counter(0)
    , off(30)
    , current(1)
    , curCount(0)
    , on(10)
    , done(false)
    , high(true)
    , blink(true)
    {}

    void loop()
    {
        if ( !done )
        {
            if ( ++counter < TOTAL )
            {
                if ( blink )
                    digitalWrite(13, HIGH);
                else
                    digitalWrite(13, LOW);
                blink = !blink;
                if ( ++curCount >= current) 
                {
                    if ( high ) {
                        current = random(on/2,on);
                        Serial.print("on ");
                        Serial.println(current);
                        digitalWrite(9, HIGH );
                        on += 1;
                    } else {
                        current = random(off/2,off);
                        digitalWrite(9, LOW);
                        Serial.print("off ");
                        Serial.println(current);
                        off -= 1;
                    }
                    high = !high;
                    curCount = 0;
                }
            }
            else
            {
                digitalWrite(9, LOW);
                digitalWrite(13,LOW);
                done = true;
            }
        }
        
    }
private:
    long counter;
    long off;
    long current;
    long curCount;
    long on;
    bool done;
    bool high;
    bool blink;
};

void
ledCallback(Task* task)
{
    ((LedTask *)task)->loop();
    
}
LedTask ledTimer(500,ledCallback);


#endif

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
	Serial.println(analogRead(A0));
    int a  = analogRead(A0);
    
    digitalWrite(13, a<20);
	delay(10);
}

