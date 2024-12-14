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
  SimpleRT rt = SimpleRT(2000);
  while (true) {
    aWarningLights();
    rt.awaitNextIteration();
  }
}


void loop() {
}