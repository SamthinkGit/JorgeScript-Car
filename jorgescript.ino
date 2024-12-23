/*
=================================================
|           [      MAIN FILE      ]             |
=================================================
*/

#include "Kalman.hpp"
#include "timer.hpp"
#include "PD.hpp"
#include "SimpleRT.hpp"


// ========== Parameters and Object Initialization ==========

// -> Kalman Filter: Used to reduce noise and track the line's position.
float R = 0.2;  // Measurement noise
float Q = 0.8;  // Process noise
float P = 1;    // Initial estimation covariance
KalmanFilter detector = KalmanFilter(0.0, P, Q, R); // Kalman filter object for line detection

// -> Default PID parameters: Control motor speeds for precise line following.
float input;            // PID input
float Kp = 0.4;         // Proportional gain
float Kd = 10;          // Derivative gain
float Ki = 0.00001;     // Integral gain
float iLimit = 10;      // Integral windup limit
float restartI = 0.8;   // Threshold to reset the integral term
PID pid(Kp, Ki, Kd, iLimit, restartI);

// -> Dynamic PID control parameters: Adjust Kp based on the situation.
float LINEAR_KP = 0.3;        // Kp for smooth operation
float AGGRESIVE_KP = 0.8;     // Kp for sharp turns

float LOST_KP = 5;            // [DEPRECATED] Kp for lost line
float CHANGE_KP_AT = 0.3;     // [DEPRECATED] Threshold to switch PID modes

// -> Obstacle detection parameters
float obstacle_distance = 0.0; // Current distance to detected obstacle
float STOP_DISTANCE = 11;      // Stop when an obstacle is within this distance
float STOP_SHIFT = 4;          // Safety margin for obstacle stopping
bool esp_ready = false;        // Flag for ESP initialization status

// -> State variables
volatile bool last_lost = false; // Tracks if the line was previously lost
volatile bool ended = false;     // Flag for end of lap
volatile bool finished = false;  // Flag for lap completion

// -> Line tracking metrics
float visible = 0;        // Number of iterations where the line is detected
float total_line = 0;     // Total iterations during navigation


// ========== System Setup ==========
void setup() {

  Serial.begin(38400);
  setupWheels();
  setupLed();
  setupSound(&obstacle_distance);
  
  // Wait for ESP to signal readiness via serial communication
  while (!esp_ready) {
    if (Serial.available()) {
      String recibido = Serial.readStringUntil('\n');
      recibido.trim();

      if ((recibido.startsWith("<") && recibido.endsWith(">"))) {
      esp_ready = true;
      }
    }
  }

  // Create and start system tasks with priorities
  SimpleRT::newTask("aNavigation", aNavigation, 1);            // Navigation task
  SimpleRT::newTask("ObstacleDetector", aObstacleDetector, 2); // Obstacle detection task
  SimpleRT::newTask("sendMessages", sendMessages, 3);          // Communication task
  SimpleRT::newTask("LED", debugLED, 5);                       // LED status task
  SimpleRT::start();
}

// ============================
// Maximum Delay: 3ms
// ============================
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
      minSpeed(-255); // Drifting
      pid.setKp(AGGRESIVE_KP);
      pid.setKi(0.001);
    } else {
      minSpeed(0);          
      pid.setKp(LINEAR_KP);  
      pid.setKi(0.0);
    }

    float position_estimate = detector.getEstimate();
    input = pid.next(position_estimate);
    setMotorSpeedsFromSlope(-input);
  }
}

// ============================
// Maximum Delay: 1ms
// ============================
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

// ============================
// Maximum Delay: 4ms
// ============================
void aObstacleDetector(void *args) {
    SimpleRT rt = SimpleRT(20);
    while (true) {
      updateDistance();
      rt.awaitNextIteration();
    }
}

// ============================
// Maximum Delay: 2ms
// ============================
void sendMessages(void *args) {
  SimpleRT rt = SimpleRT(100);
  while (true) {
    if (detector.lost() && !last_lost) {
      Serial.println("<LINE_LOST:0>");
      Serial.println("INIT_LINE_SEARCH:0");
      last_lost = detector.lost();
    }
    if (!detector.lost() && last_lost) {
      Serial.println("<LINE_FOUND:0>");
      Serial.println("STOP_LINE_SEARCH:0");
      last_lost = detector.lost();
    }
    if (obstacle_distance < STOP_DISTANCE && !ended) {
      Serial.print("<OBSTACLE_DETECTED:"); 
      Serial.print(obstacle_distance - STOP_SHIFT);
      Serial.println(">");

      Serial.print("<VISIBLE_LINE:"); 
      unsigned long perc = (visible/total_line)*100;
      Serial.print(perc);
      Serial.println(">");
      ended = true;
    }
    if (ended && !finished) {
      Serial.println("<END_LAP:0>");
      finished = true;
    }
    trackLine();
    rt.awaitNextIteration();
  }
}

// ===============================
// Maximum Delay: Nah, negligible
// ===============================
void trackLine() {
  total_line++;
  if (!detector.lost()) {
    visible++;
  }
}

void loop() {
  // Empty as tasks are managed by the scheduler
}