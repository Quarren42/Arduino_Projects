// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

const int led_pin = 3;
const int transmit_pin = 5;
const int IR_pin = 2;
//const int receive_pin = 2;
//const int transmit_en_pin = 6;

void setup()
{
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
  //  vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(4000);       // Bits per sec
    pinMode(led_pin, OUTPUT);
    pinMode(IR_pin, INPUT);
    Serial.begin(9600);
}

byte count = 1;

void loop()
{
  char msg[2] = {'O','N'};
  
  if (digitalRead(IR_pin) == HIGH){
    
  digitalWrite(led_pin, LOW); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 2);
  vw_wait_tx(); // Wait until the whole message is gone
  Serial.println("ON");
  digitalWrite(led_pin, HIGH);
  
  }
  else {
    char msg[3] = {'O','F','F'};
    vw_send((uint8_t *)msg, 3);
    vw_wait_tx();
    Serial.println("OFF");
    digitalWrite(led_pin, LOW);
  }
}
