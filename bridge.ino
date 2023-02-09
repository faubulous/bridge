#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Stepper.h>

const int LED_PIN = 13;

// Taken from here: https://arduinogetstarted.com/tutorials/arduino-controls-28byj-48-stepper-motor-using-uln2003-driver

// Number taken from here: https://www.makerguides.com/28byj-48-stepper-motor-arduino-tutorial/
const int STEPS_PER_REVOLUTION = 2048;

// Note: The order of pins is adapted to the *actual* wiring on the stepper
// motor and differs from the order describe in the previous articles.
Stepper stepper(STEPS_PER_REVOLUTION, 9, 11, 8, 10);

int position = 0;

int target = 268;

int threshold = 5;

void setup(){
  // Set the built-in LED pin as output.
  pinMode(LED_PIN, OUTPUT);

  // Initialize the serial interface with a baudrate of 9600.
  Serial.begin(9600);

  // The stepper motor needs time to settle.
  delay(1000);

  // Write a header to be able to distiguish different runs on the console.
  Serial.println();
  Serial.println("Initialized.");
}

void loop() {
  bool levelled = updatePosition();

  if(!levelled) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);

    Serial.println("LEVELLED");
  }

  step();
}

bool updatePosition() {
  // Map the 8 Bit analog value to the 6cm scale of the potentiometer.
  position = map(analogRead(A0), 0, 1024, 0, 600);

  Serial.print("position: ");
  Serial.println(position);

  int center = 300;

  return (center - threshold) < position && position < (center + threshold);
}

void step() {
  int distance = target - position;

  Serial.print("step: ");
  Serial.println(distance);

  if(abs(distance) >= threshold)
  {
    int d = distance > 0 ? -1 : 1;
    int s = d * min(100, abs(distance));

    // This is the speed I found to be most reliable. With higher
    // speeds the motor has hick-ups when running counter clock wise.
    stepper.setSpeed(11);
    stepper.step(s);

    position += s;
  }
}