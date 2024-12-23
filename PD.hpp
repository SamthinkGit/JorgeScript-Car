//Arduino
/*
=================================================
PID - Header
=================================================
*/
#include <Arduino.h>
#include <math.h>


// =================== CLASS DEFINITION ===================
#ifndef PID_HPP
#define PID_HPP

class PID {
private:
    float Kp, Ki, Kd;
    float latest;
    float integral;
    float integralLimit;
    float restartI;

public:
    PID(float Kp, float Ki, float Kd, float integralLimit, float iLimit);

    float next(float input);
    void setKp(float Kp);
    void setKi(float Ki);
    void setKd(float Kd);
};

#endif
