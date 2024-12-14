//#include <Arduino_FreeRTOS.h>  // Include the RT lib
#include "FastLED.h"
// #include "SimpleRT.hpp"

void setup() {


  Serial.begin(9600);
  Serial.println("----- STARTING -----");

  setupLed();
}

void loop() {
  warningLights();
  delay(2000);
}