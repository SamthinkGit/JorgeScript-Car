//Arduino
#include "PD.hpp"

PD::PD(float Kp, float Kd) {
    this->Kp = Kp;
    this->Kd = Kd;
    this->latest = 0.0;
}

float PD::next(float input) {
    float proportional = Kp * input;
    float derivative = Kd * (input - latest);

    latest = input;
    return proportional + derivative;
}

void PD::setKp(float Kp) {
    this->Kp = Kp;
}