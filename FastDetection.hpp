//Arduino
#include <Arduino.h>
#include <math.h>


#ifndef FAST_DETECTION_HPP
#define FAST_DETECTION_HPP

#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

const int LASER_UMBRAL = 500;

class FastDetector {
private:
    int last_last_detections[3];
    int last_detections[3];
    int current_detections[3];
    volatile bool is_lost;

public:
    float slope;

    FastDetector();
    void read();
    void loadSlope();
    bool lost();
    bool array_is(int* array, int i, int j, int k);
};


#endif