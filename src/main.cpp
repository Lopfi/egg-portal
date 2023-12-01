#include <Arduino.h>
#include <config.h>

int distance = 20; // opening distance in mm
bool state = OPEN; // current door state

void setupPins();
void setDoor(bool setState);

void setup() {
  setupPins();
}

void loop() {
  
}

void setupPins() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(neoPixelPin, OUTPUT);
}

void setDoor(bool setState) {
  if (setState == state) return;

  digitalWrite(dirPin, REVERSED ? !setState : setState);
  digitalWrite(enablePin, ENABLED);
  for (int i = 0; i < stepsPerMillimeter * distance; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  digitalWrite(enablePin, DISABLED);
  state = setState;
}