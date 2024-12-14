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
    float next(float input);
};


#endif