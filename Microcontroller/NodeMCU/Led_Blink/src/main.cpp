#include <Arduino.h>

#define BUILTIN_LED D5

void setup() {
  pinMode(BUILTIN_LED , OUTPUT);
  // put your setup code here, to run once:
}

void loop() {
  digitalWrite(BUILTIN_LED , HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED , LOW);
  delay(500);
  // put your main code here, to run repeatedly:
}
