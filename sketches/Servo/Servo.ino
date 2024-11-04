
#include <SoftTimer.h>
#include <General.h>
#include <Servo.h>
const int ServoPin = 9;
const int ButtonPin = 3;
const int debounceCount = 400;
Servo myServo;
int buttonDebounce;

uint8_t  buttonTimer,buttonCount;

void
buttonReaderCallback(Task* task) {
  if ( buttonDebounce ) {
    --buttonDebounce;
    return;
  }
  if ( !digitalRead(ButtonPin) ) {
      //Serial.println("Button");
       int pos = random(0,180);
       Serial.println(pos);
       myServo.write(pos);
       buttonDebounce = debounceCount;
  } 
}

Task buttonTimerTask(3,buttonReaderCallback);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A4));
  // done in RTC
  //SPI.begin(); 
  buttonDebounce=debounceCount;

  pinMode(ButtonPin,INPUT_PULLUP);
  myServo.attach(ServoPin);
  Serial.println("starting");
  SoftTimer.add(&buttonTimerTask);
}
