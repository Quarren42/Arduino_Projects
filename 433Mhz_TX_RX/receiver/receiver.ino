// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

const int led_pin = 6;
//const int transmit_pin = 3;
const int receive_pin = 5;
//const int transmit_en_pin = 2;
const int motor = 7;

boolean msgToggle;

void setup()
{
  delay(1000);
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");
  AFMS.begin();
  
  // Initialise the IO and ISR
  
  vw_set_rx_pin(receive_pin);
  vw_setup(4000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  
  pinMode(led_pin, OUTPUT);
  pinMode(motor, OUTPUT);
  
  myMotor -> setSpeed(255);
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {

    digitalWrite(led_pin, HIGH); // Flash a light to show received good message
    // Message with a good checksum received, dump it.

    if (msgToggle == false){
      if (buf[0] == 'O'){
        if (buf[1] == 'N'){
          Serial.println("MOVEMENT DETECTED");
          msgToggle = true;
        }
      }
    }

    if (msgToggle == true){
      if (buf[0] == 'O'){
        if (buf[1] == 'F'){
          if (buf[2] == 'F'){
            Serial.println("NO MOVEMENT DETECTED");
            msgToggle = false;
          }
        }
      }
    }
    digitalWrite(led_pin, LOW);
  }

  if (msgToggle == true){
    myMotor -> run(FORWARD);
  }
  else {
    myMotor -> run(RELEASE);
  }
}




