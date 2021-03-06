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

const int receive_pin = 8;

void setup()
{
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");
  vw_set_rx_pin(receive_pin);
  vw_setup(4000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (digitalRead(receive_pin) == LOW)
  {
    Serial.println("HIGH");
  }
}

