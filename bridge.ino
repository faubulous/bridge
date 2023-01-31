#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "Stepper.h"

int turnDirection = -1;
int lastDirection = turnDirection;

// Number taken from here: https://www.makerguides.com/28byj-48-stepper-motor-arduino-tutorial/
const int stepsPerRevolution = 1024;

Stepper stepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup(){
  // Set stepper speed to 20 rpms; this may be too slow
  // however, greater speeds are not supported.
  stepper.setSpeed(20);

  // The stepper motor needs time to settle.
  delay(1000);

  // The digital pin used for the hall sensor.
  // pinMode(2, INPUT);

  // Initialize the serial interface with a baudrate of 9600.
  Serial.begin(9600);

  // Write a header to be able to distiguish different runs on the console.
  Serial.println();
  Serial.println("Initialized.");
}

void loop() {
  // loopTest();
  loopBall();
}

void loopTest() {
  Serial.print("Direction: ");
  Serial.println(turnDirection);

  // Turn upwards and then downwards leaving a slight slope.
  turn(turnDirection * stepsPerRevolution);
  turn(turnDirection * -stepsPerRevolution);

  Serial.println("Levelled..");
  delay(5000);

  // Now turn in the opposite direction.
  turnDirection = turnDirection * -1;  
}

void loopBall() {
  Serial.print("Direction: ");
  Serial.println(turnDirection);

  // We use different turning angles for each direction because
  // the bridge turns faster when turning counter clock wise.
  // const float alpha = turnDirection == 1 ? 0.8 : 0.9;

  const int a = floor(0.95 * stepsPerRevolution);
  const int b = floor(0.8 * a);
  const int c = a - b;

  // Turn upwards and then downwards leaving a slight slope.
  int t = turn(turnDirection * a);

  t = turn(turnDirection * -b);
  t = max(10000 - t, 0);
  
  if(t > 0) {
    Serial.print("Waiting ");
    Serial.print(t);
    Serial.println("ms");
  }

  // Wait for the ball to settle..
  delay(t);

  // Level the bridge again.
  turn(turnDirection * -c);

  Serial.println("Levelled..");
  // delay(100);

  // Now turn in the opposite direction.
  turnDirection = turnDirection * -1;
}

int turn(int steps) {
  int s = steps;
  
  unsigned long t = millis();
  
  Serial.print("Turn: ");
  Serial.print(s);
  Serial.print(" steps");
  Serial.flush();

  stepper.step(s);

  int result = millis() - t;

  Serial.print("; took ");
  Serial.print(result);
  Serial.println("ms");

  return result;
}

// bool getHallSensorValue() {
//   return digitalRead(2) == 1.0;
// }

// bool isBridgeLevelled() {
//   int n = 3;

//   while(n > 0) {
//     bool result = getHallSensorValue();

//     Serial.print("isBridgeLevelled: ");
//     Serial.println(result ? "true" : "false");

//     if(!result) {
//       return false;
//     }

//     delay(1);

//     n -= 1;
//   }

//   return true;
// }

// bool testHallSensor() {
//   do {
//     bool value = getHallSensorValue();

//     Serial.print("testHallSensor: ");
//     Serial.println(value ? "true" : "false");

//     delay(1000);
//   }
//   while(true);
// }

// bool levelBridge() {
//   bool levelled = isBridgeLevelled();
//   int n = 0;

//   Serial.println(levelled);
  
//   // Rotate by max 200 steps in one direction to find zero.
//   while(!levelled && n < 400) {
//     stepper.step(5);

//     n += 5;

//     levelled = isBridgeLevelled();

//     if(levelled) {
//       return true;
//     }
//   }

//   // If we are not levelled, try to rotate the same amount in the other direction.
//   while(!levelled && -400 < n) {
//     stepper.step(-5);

//     n -= 5;

//     levelled = isBridgeLevelled();

//     if(levelled) {
//       return true;
//     }
//   }

//   return levelled;
// }
