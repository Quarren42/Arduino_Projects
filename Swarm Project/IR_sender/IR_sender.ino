#define IRpin_PIN      PIND
#define IRpin          2

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
#define NUMPULSES 50

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

// What percent we will allow in variation to match the same code
#define FUZZINESS 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing

#include "ircodes.h"

const int enMotorA = 3;
const int phaseMotorA = 4;
const int enMotorB = 5;
const int phaseMotorB = 7;

void setup(void) {
  pinMode(phaseMotorA, OUTPUT);
  pinMode(enMotorA, OUTPUT);

  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
}

void loop(void) {
  int numberpulses;

  numberpulses = listenForIR();

  Serial.print("Heard ");
  Serial.print(numberpulses);
  Serial.println("-pulse long IR signal");

  if (IRcompare(numberpulses, motorStop,sizeof(motorStop)/4)) {
    Serial.println("Stop");
    driveStop();
  }

  if (IRcompare(numberpulses, motorForwards,sizeof(motorForwards)/4)) {
    Serial.println("Forwards");
    driveForwards();
  }

  if (IRcompare(numberpulses, motorBackwards,sizeof(motorBackwards)/4)) {
    Serial.println("Backwards");
    driveBackwards();
  }

  if (IRcompare(numberpulses, motorLeft,sizeof(motorLeft)/4)) {
    Serial.println("Left");
    driveLeft();
  }

  if (IRcompare(numberpulses, motorRight,sizeof(motorRight)/4)) {
    Serial.println("Right");
    driveRight();
  }


  delay(500);
}


void driveStop(){
  analogWrite(enMotorA, 0);
  analogWrite(enMotorA, 0);
}

void driveForwards(){
  digitalWrite(phaseMotorA, HIGH);
  digitalWrite(phaseMotorB, LOW);
  analogWrite(enMotorA, 255);
  analogWrite(enMotorB, 255);
  delay(1000);
  analogWrite(enMotorA, 0);
  analogWrite(enMotorB, 0);
}  

void driveBackwards(){
  digitalWrite(phaseMotorA, LOW);
  digitalWrite(phaseMotorB, HIGH);
  analogWrite(enMotorA, 255);
  analogWrite(enMotorB, 255);
  delay(1000);
  analogWrite(enMotorA, 0);
  analogWrite(enMotorB, 0);
}  

void driveLeft(){
  digitalWrite(phaseMotorA, LOW);
  digitalWrite(phaseMotorB, LOW);
  analogWrite(enMotorA, 255);
  analogWrite(enMotorB, 255);
  delay(650);
  analogWrite(enMotorA, 0);
  analogWrite(enMotorB, 0);
}  

void driveRight(){
  digitalWrite(phaseMotorA, HIGH);
  digitalWrite(phaseMotorB, HIGH);
  analogWrite(enMotorA, 255);
  analogWrite(enMotorB, 255);
  delay(650);
  analogWrite(enMotorA, 0);
  analogWrite(enMotorB, 0);
}  

//KGO: added size of compare sample. Only compare the minimum of the two
boolean IRcompare(int numpulses, int Signal[], int refsize) {
  int count = min(numpulses,refsize);
  Serial.print("count set to: ");
  Serial.println(count);
  for (int i=0; i< count-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;

#ifdef DEBUG    
    Serial.print(oncode); // the ON signal we heard
    Serial.print(" - ");
    Serial.print(Signal[i*2 + 0]); // the ON signal we want 
#endif   

    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } 
    else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }


#ifdef DEBUG
    Serial.print("  \t"); // tab
    Serial.print(offcode); // the OFF signal we heard
    Serial.print(" - ");
    Serial.print(Signal[i*2 + 1]); // the OFF signal we want 
#endif    

    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } 
    else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }

#ifdef DEBUG
    Serial.println();
#endif
  }
  // Everything matched!
  return true;
}

int listenForIR(void) {
  currentpulse = 0;

  while (1) {
    uint16_t highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length

    //  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
      // pin is still HIGH

      // count off another few microseconds
      highpulse++;
      delayMicroseconds(RESOLUTION);

      // If the pulse is too long, we 'timed out' - either nothing
      // was received or the code is finished, so print what
      // we've grabbed so far, and then reset

      // KGO: Added check for end of receive buffer
      if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return currentpulse;
      }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;

    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
      // pin is still LOW
      lowpulse++;
      delayMicroseconds(RESOLUTION);
      // KGO: Added check for end of receive buffer
      if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return currentpulse;
      }
    }
    pulses[currentpulse][1] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
}
void printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }

  // print it in a 'array' format
  Serial.println("int IRsignal[] = {");
  Serial.println("// ON, OFF (in 10's of microseconds)");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    Serial.print("\t"); // tab
    Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
    Serial.print(", ");
    Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
    Serial.println(",");
  }
  Serial.print("\t"); // tab
  Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
  Serial.print(", 0};");
}



