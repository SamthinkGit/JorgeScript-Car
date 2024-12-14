#include "FastLED.h"
#include "SimpleRT.hpp"

void setup() {


  Serial.begin(9600);
  Serial.println("----- STARTING -----");

  setupLed();
  SimpleRT::newTask("aLoop", aLoop, 1);
  SimpleRT::start();
}

void aLoop(void *params) {
  warningLights();
  delay(2000);
}


void loop() {
}