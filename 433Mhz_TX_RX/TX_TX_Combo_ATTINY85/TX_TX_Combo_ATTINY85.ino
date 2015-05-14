#include <VirtualWire.h>
#include <SoftwareSerial.h>

const int transmit_pin = 0;
const int receive_pin = 1;
const int tx_button = 2;
//const int led_pin = 2;

const int tx = 3;
const int rx = 4;

SoftwareSerial mySerial(rx, tx);

void setup()
{
  // Initialise the IO and ISR
  delay(1000);
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  
  //pinMode(led_pin, OUTPUT);
  pinMode(tx_button, INPUT_PULLUP);
  
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);

  vw_setup(4000);       // Bits per sec
  vw_rx_start();
  mySerial.begin(9600);
}

void loop()
{

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  char msg[4] = {'T','E', 'S', 'T'};

  if (digitalRead(tx_button) == LOW){

   // digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, 4);
    vw_wait_tx(); // Wait until the whole message is gone
   // digitalWrite(led_pin, LOW);
    delay(150);

  } 
  else{

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      mySerial.print("Got: ");

      for (int i = 0; i < buflen; i++)
      {
        mySerial.print(buf[i], HEX);
        mySerial.print(' ');
      }
      mySerial.println();
    }
  }
}




