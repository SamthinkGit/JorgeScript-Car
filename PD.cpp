//Arduino
/*
=================================================
PID Controller - Module
=================================================
This module implements a Proportional-Integral-Derivative (PID)
controller, which is widely used in control systems to regulate
the velocity variable by minimizing the error over time.

Features:
  - Adjustable proportional (Kp), integral (Ki), and derivative (Kd) gains.
  - Integral windup prevention through an integral limit.
  - Reset integral term when the error is below a threshold.
  - Simple interface to compute the next control output.
*/

#include "PD.hpp"

PID::PID(float Kp, float Ki, float Kd, float integralLimit, float restartI) {
    this->Kp = Kp;                          // Proportional gain
    this->Ki = Ki;                          // Integral gain
    this->Kd = Kd;                          // Derivative gain
    this->latest = 0.0;                     // Stores the latest input value
    this->integral = 0.0;                   // Accumulated integral term
    this->integralLimit = integralLimit;    // Limits to prevent integral windup
    this->restartI = restartI;              // Threshold to reset the integral term
}

float PID::next(float input) {
    // Computes the next output of the PID controller based on the input error.
    float proportional = Kp * input;

    integral += Ki*input;
    integral = constrain(integral, -integralLimit, integralLimit);
    
    if (abs(input) < restartI) {
      integral = 0;
    }

    float derivative = Kd * (input - latest);
    latest = input;
    return proportional + integral + derivative;
}

void PID::setKp(float Kp) {
    this->Kp = Kp;
}

void PID::setKi(float Ki) {
    this->Ki = Ki;
}

void PID::setKd(float Kd) {
    this->Kd = Kd;
}
