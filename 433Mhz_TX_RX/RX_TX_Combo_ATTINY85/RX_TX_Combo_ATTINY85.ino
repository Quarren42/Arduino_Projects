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
  char radioBuffer[] = {' '};
  char serialBuffer[] = {' '};
    
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (mySerial.available()){ 
    
    mySerial.readBytesUntil('\n', radioBuffer, 1); //read what number is coming through (1,2,3, etc)
    vw_send((uint8_t *)radioBuffer, 1);
    vw_wait_tx();
  }
  
  else if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    for (int i = 0; i < buflen; i++)
    {
      serialBuffer[i] = buf[i];
      mySerial.print(serialBuffer[i]);
    }
    mySerial.println();
  }
}





