#include <Servo.h>
#include <Wire.h>

const int reedSwitch = 12;
const int servoPin = 3;
const int LED = 4;

int incomingData;
int rotations = 0;
int oldRotations;
float rotationPeriod;

unsigned long startMillis = 0;

boolean switchPrevOn = false;

void setup() {

  pinMode(reedSwitch, INPUT_PULLUP);
  pinMode(servoPin, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

}

void loop() {

  char buffer[] = {' ', ' ', ' ', ' ', ' '  };

  if (Serial.available() > 0){
    Serial.readBytesUntil('\n', buffer, 5);
    incomingData = atoi(buffer);
    Serial.println(incomingData);
  }
  if (digitalRead(reedSwitch) == LOW && switchPrevOn == false){
    digitalWrite(LED, HIGH);
    rotations++;
    rotationPeriod = millis() - startMillis;
    rotationPeriod /= 1000;
    startMillis = millis();
    
    Serial.print("rotations: ");
    Serial.print(rotations);
    Serial.print("  period: ");
    Serial.print(rotationPeriod);
    Serial.println(" seconds");
    
    switchPrevOn = true;
  }
  else if (digitalRead(reedSwitch) == HIGH && switchPrevOn == true){
    digitalWrite(LED, LOW);
    switchPrevOn = false;
  }
}













