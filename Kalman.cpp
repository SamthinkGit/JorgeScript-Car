//Arduino
/*
=================================================
Kalman Filter for Line Detection
=================================================
This module implements a Kalman filter to smooth
sensor readings and estimate the position of a
line detected by three sensors. The estimated
position ranges from -1 (left) to 1 (right).

Features:
  - Predicts the current state of the system.
  - Updates the state based on sensor measurements.
  - Handles missing or noisy data gracefully.
  - Provides an interface for detecting if the
    system has lost the line.
*/

#include "Kalman.hpp"

KalmanFilter::KalmanFilter(float init_state, float init_cov, float process_noise, float measurement_noise){
    // Initializes the Kalman filter parameters and sensor pins.
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
  // Updates the Kalman filter with a new measurement

  if (isnan(z)) {
    z = last_z;
  }

  // Calculate Kalman gain.
  float K = P * H / (H * P * H + R);

  // Update state estimate.
  x_est = x_est + K * (z - H * x_est);

  // Update estimation covariance.
  P = (1 - K * H) * P;

  // Store the current measurement.
  last_z = z;
}

void KalmanFilter::predict() {
    // Predicts the next state using the system model.

    x_est = A * x_est;  // Predict the next state.
    P = A * P * A + Q;  // Update the covariance with process noise.
}

// Returns the current estimated position of the line.
float KalmanFilter::getEstimate() {
      return x_est;
}

// Reads sensor data and converts it to a position value (-1 to 1).
float KalmanFilter::measure() {
  int detections[3];

  detections[0] = analogRead(PIN_ITR20001_LEFT);
  detections[1] = analogRead(PIN_ITR20001_MIDDLE);
  detections[2] = analogRead(PIN_ITR20001_RIGHT);


  int result[3];
  is_lost = false;

  // Determine the position of the line based on sensor patterns.
  // Note: The weights are based on the output returned from WeighedDetector

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
    return -0.75;
  }

  if (result[0] == 0 && result[1] == 0 && result[2] == 1) {
    return 0.75;
  }

  // If no line is detected, mark as lost.
  is_lost = true;
  return NAN;
}

bool KalmanFilter::lost() {
  // Returns whether the line has been lost.
  return is_lost;
}