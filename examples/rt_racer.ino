#include "WeighedDetection.hpp"
#include "timer.hpp"
#include "PD.hpp"
#include "SimpleRT.hpp"


WeighedDetector detector;
float input;

float Kp = 2;
float Kd = 8;

float LINEAR_KP = Kp;
float AGGRESIVE_KP = 3;
float CHANGE_KP_AT=0.3;

PD pd = PD(Kp, Kd);

void setup() {

  Serial.begin(9600);
  setupWheels();
  SimpleRT::newTask("aNavigation", aNavigation, 1);
  SimpleRT::start();
}

// Maximum Delay: 3ms
void aNavigation(void *args) {
  NonBlockingTimer timer;
  detector = WeighedDetector();
  SimpleRT rt = SimpleRT(10);
  
  rt.await(1000);
  timer.start();

  while (true) {

    if (timer.getElapsedTime() > 11) {
      setMotorSpeeds(0, 0);
      continue;
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
    rt.awaitNextIteration();
  }
}

void loop() {

}