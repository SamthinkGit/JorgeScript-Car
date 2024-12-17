//Arduino
#include "PD.hpp"

PID::PID(float Kp, float Ki, float Kd, float integralLimit, float restartI) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->latest = 0.0;
    this->integral = 0.0;
    this->integralLimit = integralLimit;
    this->restartI = restartI;
}


float PID::next(float input) {
    float proportional = Kp * input;

    integral += Ki*input;
    integral = constrain(integral, -integralLimit, integralLimit);
    
    if (abs(input) < restartI) {
      integral = 0;
    }

    float derivative = Kd * (input - latest);
    latest = input;

    //Serial.print("I: ");
    //Serial.print(integral);

    return proportional + integral + derivative;
}

// Métodos para ajustar las ganancias
void PID::setKp(float Kp) {
    this->Kp = Kp;
}

void PID::setKi(float Ki) {
    this->Ki = Ki;
}

void PID::setKd(float Kd) {
    this->Kd = Kd;
}
