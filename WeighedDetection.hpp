//Arduino
#include <Arduino.h>

#ifndef WEIGHED_DETECTION_HPP
#define WEIGHED_DETECTION_HPP

#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0


const int LASER_UMBRAL = 800;
const float MAX_PROB = 0.85;
const float MIN_PROB = 0.15;
const float CENTER_INFLUENCE = 2.0;
const float LATERAL_INFLUENCE = 1.0;
class WeighedDetector {

private:
    int detections[3];
    int digital_detections[3];
    float weights[3];
    float slope;

public:
    WeighedDetector();
    void read();
    void log();
    void computeProbs();
    void computeSlope();
};


#endif