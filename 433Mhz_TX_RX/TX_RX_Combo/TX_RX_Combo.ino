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

const int tx_button = 5;
const int led_pin = 4;
const int transmit_pin = 3;
const int receive_pin = 2;

void setup()
{
  // Initialise the IO and ISR
  delay(1000);
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  pinMode(led_pin, OUTPUT);
  pinMode(tx_button, INPUT_PULLUP);

  vw_setup(4000);       // Bits per sec
  vw_rx_start();//
  Serial.begin(9600);
}

void loop()
{

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  char msg[2] = {'O','N'};

  if (digitalRead(tx_button) == LOW){

    digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, 2);
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(led_pin, LOW);
    delay(150);

  } 
  else{

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      Serial.print("Got: ");

      for (int i = 0; i < buflen; i++)
      {
        Serial.print(buf[i], HEX);
        Serial.print(' ');
      }
      Serial.println();
    }
  }
}




