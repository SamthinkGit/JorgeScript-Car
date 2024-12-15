//Arduino

#include <Arduino.h>
#ifndef PD_HPP
#define PD_HPP
class PD {

private:
  float Kp; 
  float Kd;  
  float latest;

public:
    PD(float Kp, float Kd);
    void setKp(float Kp);
    float next(float input);
};


#endif