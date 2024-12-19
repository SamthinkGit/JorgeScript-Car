#include "Kalman.hpp"
#include "timer.hpp"
#include "PD.hpp"
#include "SimpleRT.hpp"


float R = 0.2;
float Q = 0.8;
float P = 1;
KalmanFilter detector = KalmanFilter(0.0, P, Q, R);
float input;

float Kp = 0.4;
float Kd = 10;
float Ki = 0.00001;
float iLimit = 10;
float restartI = 0.8;

float LINEAR_KP = 0.3;  
float AGGRESIVE_KP = 0.6
;
float LOST_KP = 5;
float CHANGE_KP_AT=0.3;
float obstacle_distance = 0.0;
float STOP_DISTANCE = 20;
bool esp_ready = false;
volatile bool last_lost = false;
volatile bool ended = false;
volatile bool finished = false;

float visible = 0;
float total_line = 0;

PID pid(Kp, Ki, Kd, iLimit, restartI);

void setup() {

  Serial.begin(38400);
  setupWheels();
  setupLed();
  setupSound(&obstacle_distance);
  
  while (!esp_ready) {
    if (Serial.available()) {
      String recibido = Serial.readStringUntil('\n');
      recibido.trim();

      if ((recibido.startsWith("<") && recibido.endsWith(">"))) {
      esp_ready = true;
      }
    }
  }

  SimpleRT::newTask("aNavigation", aNavigation, 1);
  SimpleRT::newTask("ObstacleDetector", aObstacleDetector, 2);
  SimpleRT::newTask("sendMessages", sendMessages, 3);
  SimpleRT::newTask("LED", debugLED, 5);
  SimpleRT::start();
}

// Maximum Delay: 3ms
void aNavigation(void *args) {
  NonBlockingTimer timer;
  SimpleRT rt = SimpleRT(20);
  bool stopped = false;
  timer.start();

  while (true) {
    rt.awaitNextIteration();
    if (stopped || obstacle_distance < STOP_DISTANCE) {
      setMotorSpeeds(0, 0);
      stopped = true;
      continue;
    }

    float measurement = detector.measure();
    detector.predict();
    detector.update(measurement);
    
    if (detector.lost()) {
      minSpeed(-255);
      pid.setKp(AGGRESIVE_KP);
      pid.setKi(0.001);
    } else {
      minSpeed(0);
      pid.setKp(LINEAR_KP);
      pid.setKi(0.0);
    }

    float position_estimate = detector.getEstimate();
    input = pid.next(position_estimate);
    // input = position_estimate;
    setMotorSpeedsFromSlope(-input);
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

// Maximum Delay: 4ms
void aObstacleDetector(void *args) {
    SimpleRT rt = SimpleRT(20);
    while (true) {
      updateDistance();
      rt.awaitNextIteration();
    }
}

void sendMessages(void *args) {
  SimpleRT rt = SimpleRT(100);
  while (true) {
    if (detector.lost() && !last_lost) {
      Serial.println("<LINE_LOST:0>");
      Serial.println("<INIT_LINE_SEARCH:0>");
      last_lost = detector.lost();
    }
    if (!detector.lost() && last_lost) {
      Serial.println("<LINE_FOUND:0>");
      Serial.println("<STOP_LINE_SEARCH:0>");
      last_lost = detector.lost();
    }
    if (obstacle_distance < STOP_DISTANCE && !ended) {
      Serial.println("<END_LAP:0>");
      ended = true;
    }
    if (ended && !finished) {
      Serial.print("<OBSTACLE_DETECTED:"); 
      Serial.print(obstacle_distance);
      Serial.println(">");

      Serial.print("<VISIBLE_LINE:"); 
      unsigned long perc = (visible/total_line)*100;
      Serial.print(perc);
      Serial.println(">");
      finished = true;
    }
    trackLine();
    rt.awaitNextIteration();
  }
}

void trackLine() {
  total_line++;
  if (!detector.lost()) {
    visible++;
  }
}

void loop() {

}