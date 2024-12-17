#include "Kalman.hpp"

KalmanFilter::KalmanFilter(float init_state, float init_cov, float process_noise, float measurement_noise){
    pinMode(PIN_ITR20001_LEFT, INPUT);
    pinMode(PIN_ITR20001_MIDDLE, INPUT);
    pinMode(PIN_ITR20001_RIGHT, INPUT);
    A = 1.0;
    H = 1.0;
    Q = process_noise;
    R = measurement_noise;
    P = init_cov;
    x_est = init_state;
    last_z = init_state;
}

void KalmanFilter::update(float z) {
  if (isnan(z)) {
    z = last_z;
  }
  float K = P * H / (H * P * H + R); // Ganancia de Kalman
  x_est = x_est + K * (z - H * x_est);
  P = (1 - K * H) * P;
  last_z = z;
}

void KalmanFilter::predict() {
  x_est = A * x_est;
  P = A * P * A + Q;
}

float KalmanFilter::getEstimate() {
      return x_est;
}

float KalmanFilter::measure() {
  int detections[3];

  detections[0] = analogRead(PIN_ITR20001_LEFT);
  detections[1] = analogRead(PIN_ITR20001_MIDDLE);
  detections[2] = analogRead(PIN_ITR20001_RIGHT);
  // Serial.print("[");
  // Serial.print(detections[0]);
  // Serial.print(", ");
  // Serial.print(detections[1]);
  // Serial.print(", ");
  // Serial.print(detections[2]);
  // Serial.print("] ");

  int result[3];
  is_lost = false;
  for (int i = 0; i < 3; i++) {
    result[i] = detections[i] > LASER_UMBRAL ? 1 : 0;
  }

  if (result[0] == 0 && result[1] == 1 && result[2] == 0) {
    return 0.0;
  }

  if (result[0] == 1 && result[1] == 1 && result[2] == 0) {
    return -0.2;
  }

  if (result[0] == 0 && result[1] == 1 && result[2] == 1) {
    return 0.2;
  }


  if (result[0] == 1 && result[1] == 0 && result[2] == 0) {
    return -1.0;
  }

  if (result[0] == 0 && result[1] == 0 && result[2] == 1) {
    return 1.0;
  }

  is_lost = true;
  return NAN;
}

bool KalmanFilter::lost() {
  return is_lost;
}