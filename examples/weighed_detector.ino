#include "SimpleRT.hpp"
#include "WeighedDetection.hpp"

void setup() {


  Serial.begin(9600);
  Serial.println("----- STARTING -----");

  SimpleRT::newTask("aLoop", aLoop, 1);
  SimpleRT::start();

}

void aLoop(void *params) {
  WeighedDetector detector = WeighedDetector();
  SimpleRT rt = SimpleRT(200);

  while(true) {
    detector.read();
    detector.computeProbs();
    detector.log();
    detector.computeSlope();
    rt.awaitNextIteration();
  }
}


void loop() {
}
