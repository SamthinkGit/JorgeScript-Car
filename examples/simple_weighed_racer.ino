#include "WeighedDetection.hpp"
#include "timer.hpp"

NonBlockingTimer timer;
WeighedDetector detector;

void setup() {

  Serial.begin(9600);
  timer.start();
  setupWheels();
  detector = WeighedDetector();
}

void loop() {

  if (timer.getElapsedTime() > 4) {
    setMotorSpeeds(0, 0);
    return;
  } 

  detector.read();
  detector.computeProbs();
  detector.computeSlope();
  setMotorSpeedsFromSlope(detector.slope*2.5);
}