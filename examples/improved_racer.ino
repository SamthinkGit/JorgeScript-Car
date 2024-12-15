#include "WeighedDetection.hpp"
#include "timer.hpp"
#include "PD.hpp"

NonBlockingTimer timer;
WeighedDetector detector;
float input;

float Kp = 2;
float Kd = 8;

float LINEAR_KP = Kp;
float AGGRESIVE_KP = 3;
float CHANGE_KP_AT=0.3;

PD pd = PD(Kp, Kd);

void setup() {

  delay(1000);

  Serial.begin(9600);
  timer.start();
  setupWheels();
  detector = WeighedDetector();
}

void loop() {

  if (timer.getElapsedTime() > 11) {
    setMotorSpeeds(0, 0);
    return;
  }

  detector.read();
  detector.computeProbs();
  detector.applySigmoid();
  detector.computeSlope();

  if (abs(detector.slope) > CHANGE_KP_AT) {
    pd.setKp(AGGRESIVE_KP);
  } else {
    pd.setKp(LINEAR_KP);
  }
  // detector.log();
  input = pd.next(detector.slope);
  setMotorSpeedsFromSlope(input);
}
