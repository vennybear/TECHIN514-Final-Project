/* 
 * Door Detective - XIAO ESP32C3 Test Code
 * Author: Yuna Xiong
 */

#include <Stepper.h>

Stepper myStepper(600, 2, 4, 3, 5);

const int ledPin = 6; 
const int buttonPin = 7; 

void setup() {
  Serial.begin(115200);
  myStepper.setSpeed(30); 
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);
    myStepper.step(100); 
    delay(1000);
    myStepper.step(-100); 
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}
