

uint8_t hour;
uint8_t minute;
uint8_t second;
const int HOUR_METER_PIN =3;
const uint8_t MAX_HOUR = 240;
const uint8_t MINUTE_METER_PIN = 5;
const uint8_t MAX_MINUTE = 240;
const uint8_t SECOND_METER_PIN = 6;
const uint8_t MAX_SECOND = 240;

const uint8_t hourTable[] = 
{
  0
  ,15  //1
  ,35  //2
  ,55  //3
  ,75  //4
  ,100  //5
  ,120  //6
  ,140  //7
  ,165  //8
  ,186  //9
  ,205  //10
  ,225  //11
  ,240  //12
};
const uint8_t minTable[] =
{
  0,4,8,12,16,20,24,28,32,36       // 0
  ,40,44,48,52,56,60,64,68,72,76       // 10
  ,80,84,88,92,96,100,104,108,112,116 // 20
  ,120,124,128,132,136,140,144,148,152,156 // 30
  ,160,164,168,172,176,180,184,188,192,196 // 40
  ,200,204,208,212,216,220,224,228,232,236 // 40
  ,240
};
void
displayHour(uint8_t hour)
{
  analogWrite(HOUR_METER_PIN, hourTable[hour]);
}

void
displayMinute(uint8_t min)
{
  analogWrite(MINUTE_METER_PIN, minTable[min] );
}

void
displaySecond(uint8_t sec)
{
  analogWrite(SECOND_METER_PIN, (MAX_SECOND/60)  * sec);
}
 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin led_builtin as an output.
  pinMode(HOUR_METER_PIN, OUTPUT);
  pinMode(MINUTE_METER_PIN, OUTPUT);
  pinMode(SECOND_METER_PIN, OUTPUT);
  Serial.begin(9600);
  hour = 1;
  minute = 0;
  second = 0;
}



// the loop function runs over and over again forever
void loop() {
  ++hour;
  if ( hour == 13 )
    hour = 1;
  ++minute;
  if ( minute == 60 )
    minute = 0;
  ++second;
  if ( second == 60 )
    second = 0;
  displayHour(hour);
  displayMinute(minute);
  displaySecond(second);
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);
  Serial.print(":");
  Serial.println(second);
  delay(1000);                       // wait for a second
}
