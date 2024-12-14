#include "SimpleRT.hpp"

void setup() {


  Serial.begin(9600);
  Serial.println("----- STARTING -----");

  setupDetection();
  SimpleRT::newTask("aLoop", aLoop, 1);
  SimpleRT::start();

}

void aLoop(void *params) {
  while(true) {
    int* reading = read();
    printValues(reading);
  }
}


void loop() {
}