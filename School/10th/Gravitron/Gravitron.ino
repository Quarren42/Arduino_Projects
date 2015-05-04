#include <Servo.h>
#include <Wire.h>

const int reedSwitch = 12;
const int LED = 4;
const int button = 2;

int incomingData;
float rotations = 0;
float rotationPeriod;

unsigned long startMillis = 0;

boolean switchPrevOn = false;

Servo stopServo;

void setup() {

  pinMode(reedSwitch, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  stopServo.attach(3);
  Serial.begin(9600);

}

void loop() {

  char buffer[] = {' ', ' ', ' ', ' ', ' '};

  if (Serial.available() > 0){
    Serial.readBytesUntil('\n', buffer, 5);
    incomingData = atoi(buffer);
    Serial.println(incomingData);
  }
  if (digitalRead(reedSwitch) == LOW && switchPrevOn == false){
    digitalWrite(LED, HIGH);
    rotations += 0.5;
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

  if (rotationPeriod > 5){
    stopServo.write(89); //write 89 because it jitters at 90
   delay(15);
  }
 while (digitalRead(button) == LOW){
  stopServo.write(0);
  delay(15);
 } 
}














