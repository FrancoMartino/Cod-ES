#include "LedRGB.h"
#include <Arduino.h>

LedRGB::LedRGB(int redPin, int greenPin, int bluePin) {
  rPin = redPin;
  pinMode(rPin, OUTPUT);
  gPin = greenPin;
  pinMode(gPin, OUTPUT);
  bPin = bluePin;
  pinMode(bPin, OUTPUT);
}

void LedRGB::encender() {
  analogWrite(rPin, rValue);
  analogWrite(gPin, gValue);
  analogWrite(bPin, bValue);
}

void LedRGB::apagar() {
  digitalWrite(rPin, LOW);
  digitalWrite(gPin, LOW);
  digitalWrite(bPin, LOW);
}

void LedRGB::cambiarColor(int red, int green, int blue) {
  rValue = red;
  gValue = green;
  bValue = blue;
  analogWrite(rPin, rValue);
  analogWrite(gPin, gValue);
  analogWrite(bPin, bValue);
}
