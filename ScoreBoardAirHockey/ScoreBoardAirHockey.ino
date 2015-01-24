//#include <VoiceShield.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include <Wire.h>

Adafruit_7segment matrix = Adafruit_7segment();

const int switch1 = 11; //pinout for the three switches
const int switch2 = 12;
const int button1 = 13; 

int maxScore = 14; //default winning score is 14

int val1 = 0; //reads digitral switches
int val2 = 0;
int val3 = 0;
int score1 = 0; //score for P1
int score2 = 0; //score for P2
int displayOut; //integer that is sent to the displays

//VoiceShield vs(10);

void setup()
{
  matrix.begin(0x70);
  Serial.begin(9600);
}


void loop()
{
  //vs.ISDPLAY(0);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);

  val1 = digitalRead(switch1); //read switches on each side
  val2 = digitalRead(switch2);
  val3 = digitalRead(button1); //read mode button

  switch (val1) { //add 100 to display score when switch one is pressed
  case 0:
    displayOut = displayOut + 100;
    score1 ++;
    delay(400);
    break;
  }   
  switch (val2) { //add 1 to display score when two is pressed
  case 0:
    displayOut = displayOut + 1;
    score2 ++;
    delay(400);
    break;
  }

  while ((val3 == 0) && ((score1 == 0) && score2 == 0)){ //while mode button is pressed and both scores are 0 (to set winning score)
    val1 = digitalRead(switch1); //read switches on each side
    val2 = digitalRead(switch2);
    val3 = digitalRead(button1); //read mode button status

    switch (val1) { //subtract 1 from maxscore
    case 0:
      maxScore --;
      break;
    }
    switch (val2) { //add 1 to maxscore
    case 0:
      maxScore ++;
      break;
    }
    matrix.print(maxScore); //print out the winning score
    matrix.drawColon(false); //draw the colon
    matrix.writeDisplay(); //update the display
    delay (150);
  }

  while ((val3 == 0) && ((score1 > 0) || (score2 > 0))){ //while mode button is pressed and either scores are above 0 (to subtract score in case of screw up)
    val1 = digitalRead(switch1); //read switches on each side
    val2 = digitalRead(switch2);
    val3 = digitalRead(button1); //read mode button status

    switch (val1) { //SUBTRACT when switch is pressed
    case 0:
      displayOut = displayOut - 100;
      score1 --;
      break;
    }   
    switch (val2) { 
    case 0:
      displayOut = displayOut - 1;
      score2 --;
      break;
    }

    matrix.print(displayOut); //print out score
    matrix.writeDisplay(); //update display
    delay(150);
  }

  matrix.print(displayOut); //print out the score
  matrix.drawColon(true); //draw colon
  matrix.writeDisplay(); //update display

    while ((score1 == maxScore) || (score2 == maxScore)){ //if score is (), blink the screen.
    matrix.blinkRate(1);
    matrix.writeDisplay();
    
  }
}

























