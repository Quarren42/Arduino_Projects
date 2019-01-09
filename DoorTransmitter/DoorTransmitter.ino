#include <VirtualWire.h>

const int switch_pin = 2;
const int tx_pin = 3;
boolean doorOpen = false;


void setup() {
  // put your setup code here, to run once:
  vw_set_tx_pin(tx_pin);
  vw_set_ptt_inverted(true);
  pinMode(switch_pin, INPUT_PULLUP);
  vw_setup(2000);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  const char *msg = "Y";
  const char *msg2 = "N";
  if (digitalRead(switch_pin) == HIGH)
  {
    doorOpen = true;
  } else {
    doorOpen = false;
  }
  if (doorOpen == true)
  {
    digitalWrite(13, HIGH);
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    digitalWrite(13, LOW);
  } else {
    vw_send((uint8_t *)msg2, strlen(msg2));
    vw_wait_tx();
  }
  delay(150);
}
