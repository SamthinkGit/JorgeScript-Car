//Arduino
#include <Arduino.h>
#include <math.h>


#ifndef WEIGHED_DETECTION_HPP
#define WEIGHED_DETECTION_HPP

#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0


const int LASER_UMBRAL = 100;
const float MAX_WEIGHT = 0.8;
const float MIN_WEIGHT = 0.2;
const float CENTER_INFLUENCE = 2.0;
const float LATERAL_INFLUENCE = 1.0;

const float WEIGHT_DESCEND = -0.25;
const float WEIGHT_ASCEND = 0.45;
const float SIGMOID_CONTRAST = 10;
const float MIN_SLOPE = 0.03;

const float SIGMOID_SHIFT = 0.5;
class WeighedDetector {

private:
    int detections[3];
    int digital_detections[3];
    float weights[3];

public:
    float slope;
    WeighedDetector();
    void read();
    void log();
    void computeProbs();
    void applySigmoid();
    void computeSlope();
};


#endif