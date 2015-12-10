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
  char serialBuffer[] = {' ', ' ', ' '};
  char radioBuffer[] = {' ', ' ', ' '};
    
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (mySerial.available()){ 
    mySerial.readBytesUntil('\n', serialBuffer, 3); //read what number is coming through (1,2,3, etc)
    int incomingByte = atoi(serialBuffer);
    String data = String(incomingByte, HEX);
    data.toCharArray(radioBuffer, 3);

    vw_send((uint8_t *)radioBuffer, 3);
    vw_wait_tx();
  }

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





