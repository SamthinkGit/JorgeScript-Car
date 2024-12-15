#include "WeighedDetection.hpp"
#include "timer.hpp"
#include "PD.hpp"
#include "SimpleRT.hpp"


WeighedDetector detector;
float input;

float Kp = 2;
float Kd = 8;

float LINEAR_KP = Kp;
float AGGRESIVE_KP = 2.5;
float LOST_KP = 6;
float CHANGE_KP_AT=0.3;

PD pd = PD(Kp, Kd);

void setup() {

  Serial.begin(9600);
  setupWheels();
  setupLed();
  detector = WeighedDetector();
  
  SimpleRT::newTask("aNavigation", aNavigation, 1);
  SimpleRT::newTask("LED", debugLED, 5);
  SimpleRT::start();
}

// Maximum Delay: 3ms
void aNavigation(void *args) {
  NonBlockingTimer timer;
  SimpleRT rt = SimpleRT(10);
  
  rt.await(1000);
  timer.start();

  while (true) {
    rt.awaitNextIteration();
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
    if (detector.lost()) {
      pd.setKp(LOST_KP);
    }

    // detector.log();
    input = pd.next(detector.slope);
    setMotorSpeedsFromSlope(input);
  }
}

// Maximum Delay: 1ms
void debugLED(void *args) {
  SimpleRT rt = SimpleRT(50);
  bool latest = false;
  bool new_state;
  showColor(0, 255, 0);

  while (true) {
    rt.awaitNextIteration();

    new_state = detector.lost();
    if (new_state == latest) {
      continue;
    }
    latest = new_state;

    if (detector.lost()) {
      showColor(255, 0, 0);
    } else {
      showColor(0, 255, 0);
    }
  }
}
void loop() {

}