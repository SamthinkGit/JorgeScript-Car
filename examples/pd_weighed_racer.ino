#include "WeighedDetection.hpp"
#include "timer.hpp"
#include "PD.hpp"

NonBlockingTimer timer;
WeighedDetector detector;
float input;

float Kp = 2;
float Kd = 8;
PD pd = PD(Kp, Kd);

void setup() {

  delay(1000);

  Serial.begin(9600);
  timer.start();
  setupWheels();
  detector = WeighedDetector();
}

void loop() {

  if (timer.getElapsedTime() > 3) {
    setMotorSpeeds(0, 0);
    return;
  } 

  detector.read();
  detector.computeProbs();
  detector.computeSlope();

  input = pd.next(detector.slope);
  setMotorSpeedsFromSlope(input);
}