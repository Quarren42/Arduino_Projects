#include <PID_AutoTune_v0.h>
#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

double Setpoint, Input, Output;

//PIN's definition
#define encoder0PinA  2
#define encoder0PinB  3


volatile int encoder0Pos = 0;
volatile boolean PastA = 0;
volatile boolean PastB = 0;

const int leftLimitSwitch = 4;
const int rightLimitSwitch = 5;

int Kp, Ki, Kd, Pu, Ku;
// int a_Kp, a_Ki, a_Kd;

//Kp = 0.6 * Ku;
//Ki = (2 * Kp); 
//Kd = (Kp * Pu) / 8;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// PID_ATune aTune(&Input, &Output);

void setup() 
{
  AFMS.begin();
  
  pinMode(encoder0PinA, INPUT);
  //turn on pullup resistor
  //digitalWrite(encoder0PinA, HIGH); //ONLY FOR SOME ENCODER(MAGNETIC)!!!! 
  pinMode(encoder0PinB, INPUT); 
  pinMode(leftLimitSwitch, INPUT_PULLUP);
  pinMode(rightLimitSwitch, INPUT_PULLUP);
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
  
 myPID.SetOutputLimits(-255, 255);
 myPID.SetMode(AUTOMATIC);
 myPID.Compute(); 
 myPID.SetSampleTime(100);
  
  Setpoint = 2048;
  
Kp = 1;
Ki = 0;
Kd = 0;

}

void loop()
{  
   Input = encoder0Pos;
   
  //myPID.SetTunings(Kp, Ki, Kd);
  myPID.Compute(); 
  
  if ((leftLimitSwitch || rightLimitSwitch) == 0){
    myMotor->setSpeed(0);
    myMotor->run(RELEASE);
  }
  else if (Input > 0){
    myMotor->setSpeed(Output);
  myMotor->run(FORWARD);
  }
  else if (Input < 0){
    myMotor->setSpeed(Output);
    myMotor->run(BACKWARD);
  }
}


void doEncoderA()
{
     PastB ? encoder0Pos--:  encoder0Pos++;
     Serial.print(encoder0Pos, Output);
    
 
}

void doEncoderB()
{
     PastB = !PastB; 
}

