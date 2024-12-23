//Arduino
/*
=================================================
[DEPRECATED] Fast Line Detection
=================================================
This module implements a fast line detection algorithm
using three sensors. It tracks the current and previous
sensor states to estimate the line's slope and determine
if the line is lost.
*/

#include "FastDetection.hpp"

FastDetector::FastDetector() {
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);

  last_last_detections[0] = 0;
  last_last_detections[1] = 0;
  last_last_detections[2] = 0;

  last_detections[0] = 0;
  last_detections[1] = 0;
  last_detections[2] = 0;

  current_detections[0] = 0;
  current_detections[1] = 0;
  current_detections[2] = 0;
  slope = 0;
  is_lost = false;
}

void FastDetector::read() {
  // Reads sensor data and updates the detection history.
  int detections[3];

  detections[0] = analogRead(PIN_ITR20001_LEFT);
  detections[1] = analogRead(PIN_ITR20001_MIDDLE);
  detections[2] = analogRead(PIN_ITR20001_RIGHT);
  int result[3];
  
  for (int i = 0; i < 3; i++) {
    result[i] = detections[i] > LASER_UMBRAL ? 1 : 0;
  }

  if (result[0] == 0 && result[1] == 0 && result[2] == 0) {
    current_detections[0] = last_detections[0];
    current_detections[1] = last_detections[1];
    current_detections[2] = last_detections[2];
    return;
  }
  
  for (int i = 0; i < 3; i++) {
    last_last_detections[i] = last_detections[i];
    last_detections[i] = current_detections[i];
    current_detections[i] = result[i];
  }
}


bool FastDetector::array_is(int* array, int i, int j, int k) {
  // Helper function: Checks if an array matches a specific pattern.
  return (array[0] == i && array[1] == j && array[2] == k);
}

void FastDetector::loadSlope() {
  // Calculates the slope of the line based on the current detections.
  is_lost = false;
  if (array_is(current_detections, 1, 1, 1) || array_is(current_detections, 0, 1, 0)) {
    slope = 0.0;
    return;
  } 

  if (this->array_is(current_detections, 0, 0, 1)) {
    slope = -3.0;
    return;
  } 

  if (this->array_is(current_detections, 1, 0, 0)) {
    slope = 3.0;
    return;
  }

  if (this->array_is(current_detections, 1, 1, 0)) {
    slope = 0.4;
    return;
  }
  if (this->array_is(current_detections, 0, 1, 1)) {
    slope = -0.4;
    return;
  }
  if (this->array_is(current_detections, 0, 0, 0)) {
    is_lost = true;
    if (this->array_is(last_detections, 0, 0, 1) || this->array_is(last_detections, 1, 0, 0)) {
      return;
    }
    if (this->array_is(last_detections, 0, 1, 1) || this->array_is(last_detections, 1, 1, 0)) {
      slope *= 3;
      return;
    }
    if (this->array_is(last_detections, 0, 1, 0)) {
      if (this->array_is(last_last_detections, 0, 0, 1) || (this->array_is(last_last_detections, 0, 1, 1))) {
        slope = -3.0;
        return;
      }
      if (this->array_is(last_last_detections, 1, 0, 0) || (this->array_is(last_last_detections, 1, 1, 0))) {
        slope = 3.0;
        return;
      }
    }
    slope = 3.0;
  }
}

bool FastDetector::lost() {
  return is_lost;
}
