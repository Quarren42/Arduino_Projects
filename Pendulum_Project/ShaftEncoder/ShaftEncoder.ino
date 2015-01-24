#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

//PIN's definition
#define encoder0PinA  2
#define encoder0PinB  3


volatile int encoder0Pos = 0;
volatile boolean PastA = 0;
volatile boolean PastB = 0;

const int leftLimitSwitch = 4;
const int rightLimitSwitch = 5;

void setup() 
{
  AFMS.begin();
  
  pinMode(encoder0PinA, INPUT);
  //turn on pullup resistor
  //digitalWrite(encoder0PinA, HIGH); //ONLY FOR SOME ENCODER(MAGNETIC)!!!! 
  pinMode(encoder0PinB, INPUT); 
  //turn on pullup resistor
  //digitalWrite(encoder0PinB, HIGH); //ONLY FOR SOME ENCODER(MAGNETIC)!!!! 
  PastA = (boolean)digitalRead(encoder0PinA); //initial value of channel A;
  PastB = (boolean)digitalRead(encoder0PinB); //and channel B
  Serial.begin(9600);
//To speed up even more, you may define manually the ISRs
// encoder A channel on interrupt 0 (arduino's pin 2)
  attachInterrupt(0, doEncoderA, RISING);
// encoder B channel pin on interrupt 1 (arduino's pin 3)
  attachInterrupt(1, doEncoderB, CHANGE); 
  
  myMotor->setSpeed(10); 

}


void loop()
{  
 //your stuff....ENJOY! :D
 myMotor->run(FORWARD); 
 delay(3000);
  myMotor->run(BACKWARD);  
  delay(3000);
}

//you may easily modify the code  get quadrature..
//..but be sure this whouldn't let Arduino back! 
void doEncoderA()
{
     PastB ? encoder0Pos--:  encoder0Pos++;
     Serial.println(encoder0Pos);
}

void doEncoderB()
{
     PastB = !PastB; 
}

