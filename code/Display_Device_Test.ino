/* 
 * Door Detective - Display Device Test Code
 * Author: Yuna Xiong
 */

#include <Stepper.h>

// X27.168 Stepper Motor settings
const int stepsPerRevolution = 600; 
Stepper myStepper(stepsPerRevolution, 14, 26, 27, 25);

const int buttonPin = 33;
const int ledPin = 32;

void setup() {
  Serial.begin(115200);
  myStepper.setSpeed(30); // X27 motors move better at lower speeds
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);
    myStepper.step(100); // Move needle forward
    delay(1000);
    myStepper.step(-100); // Move needle back
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}
