#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Stepper.h>

const int LED_PIN = 13;

// The stepper motor speed in rounds per minute.
const int SPEED_RPM = 11;

// See: https://arduinogetstarted.com/tutorials/arduino-controls-28byj-48-stepper-motor-using-uln2003-driver
// See: https://www.makerguides.com/28byj-48-stepper-motor-arduino-tutorial/
const int STEPS_PER_REVOLUTION = 2048;

// Note: The order of pins is adapted to the *actual* wiring on the stepper
// motor and differs from the order describe in the previous articles.
Stepper stepper(STEPS_PER_REVOLUTION, 9, 11, 8, 10); // 5V stepper motor
// Stepper stepper(STEPS_PER_REVOLUTION, 11, 9, 10, 8); // 12V stepper motor

// Potentiometer value when the bridge is levelled.
int center = 268;

// Defines potentiometer values that should be moved to in the given sequence.
// When the last value was reached the list will skip the first value and repeat from the second.
int targetList[5] = { 0, 155, 50, -155, -50 };

// Defines delays in ms that should be waited after a target was reached. The
// sequence corresponds to the values in the above target list.
int targetDelay[5] = { 5000, 0, 6500, 0, 6500 };

// Index from 0 to len(targetList) - 1 of the currently selected target.
int targetIndex = 0;

// Current value of the potentiometer mapped to mm above 0.
int position = 0;

// Current distance from the current target in mm. 
int distance = 0;

// Tolerance in mm when we consider a target to be reached.
int threshold = 3;

void setup()
{
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

void loop()
{
  updatePosition();
  updateDistance();
  step();
}

void updatePosition()
{
  // Map the 8 Bit analog value to the 6cm scale of the potentiometer.
  position = map(analogRead(A0), 0, 1024, 0, 600);

  Serial.print("position: ");
  Serial.print(position);

  bool levelled = (center - threshold) < position && position < (center + threshold);

  if(!levelled)
  {
    digitalWrite(LED_PIN, LOW);
  }
  else
  {
    digitalWrite(LED_PIN, HIGH);
  }
}

void updateDistance()
{
  int t = center + targetList[targetIndex];
  int d = t - position;

  Serial.print(" distance: ");
  Serial.println(d);

  distance = d;
}

void step()
{
  if(abs(distance) > threshold)
  {
    // Get the required direction towards the target; -1 is CCW and 1 is CW.
    int d = distance > 0 ? 1 : -1;
    int s = d * min(100, abs(distance));

    // This is the speed I found to be most reliable. With higher
    // speeds the motor has hickups when running counter clock wise (CCW).
    stepper.setSpeed(SPEED_RPM);
    stepper.step(s);

    position += s;
  }
  else
  {
    // Wait the defined number of ms for the current target..
    int ms = targetDelay[targetIndex];

    delay(ms);

    // C++ is ugly and we need to calculate the size of the target list.
    int targetCount = sizeof(targetList) / sizeof(targetList[0]);

    targetIndex += 1;

    // If we reached the last target then restart and skip the intial center.
    if(targetIndex > (targetCount - 1))
    {
      targetIndex = 1;
    }
  }
}