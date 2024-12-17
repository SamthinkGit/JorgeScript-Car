//Arduino
#include <Arduino.h>
#include <math.h>

#ifndef KALMAN_HPP
#define KALMAN_HPP

#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

const int LASER_UMBRAL = 700;

class KalmanFilter {
  private:
    float A;
    float H;
    float Q;
    float R;
    float P;
    float x_est;
    bool is_lost;
    float last_z;

  public:
    KalmanFilter(float init_state, float init_cov, float process_noise, float measurement_noise);
    float measure();
    void predict();
    void update(float z);
    float getEstimate();
    bool lost();

};

#endif