//Arduino
#include "WeighedDetection.hpp"

WeighedDetector::WeighedDetector() {
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);
  weights[0] = MAX_WEIGHT;
  weights[1] = MAX_WEIGHT;
  weights[2] = MAX_WEIGHT;
  detections[0] = 0;
  detections[1] = 0;
  detections[2] = 0;
  digital_detections[0] = 0;
  digital_detections[1] = 0;
  digital_detections[2] = 0;
  slope = 0;
}

void WeighedDetector::read() {
  detections[0] = analogRead(PIN_ITR20001_LEFT);
  detections[1] = analogRead(PIN_ITR20001_MIDDLE);
  detections[2] = analogRead(PIN_ITR20001_RIGHT);
  int result[3];
  
  for (int i = 0; i < 3; i++) {
    result[i] = detections[i] > LASER_UMBRAL ? 1 : 0;
  }

  // If all the detections fail, then we keep the latest state
  if (result[0] == 0 && result[1] == 0 && result[2] == 0) {
    return;
  }

  for (int i = 0; i < 3; i++) {
    digital_detections[i] = result[i];
  }
}

void WeighedDetector::computeProbs() {
  float addition;
  for (int i = 0; i < 3; i++) {
    addition = digital_detections[i] == 1 ? WEIGHT_ASCEND : WEIGHT_DESCEND;
    weights[i] = constrain((weights[i] + addition), MIN_WEIGHT, MAX_WEIGHT);
  }
}

void WeighedDetector::computeSlope() {
  if (weights[0] > weights[2]) {
    slope = -(CENTER_INFLUENCE*(weights[1] - weights[0]) + LATERAL_INFLUENCE*(weights[2] - weights[1])) / 3;
  } else {
    slope = (CENTER_INFLUENCE*(weights[1] - weights[2]) + LATERAL_INFLUENCE*(weights[0] - weights[1])) / 3;
  }
  if (abs(slope) < MIN_SLOPE) {
    slope = 0.0;
  }
}

void WeighedDetector::applySigmoid() {
  for (int i = 0; i < 3; i++) {
    if (weights[i] < 0) {
      weights[i] = 1/(1 + pow(M_E, SIGMOID_CONTRAST*(SIGMOID_SHIFT+weights[i])));
    } else {
      weights[i] = 1/(1 + pow(M_E, -SIGMOID_CONTRAST*(-SIGMOID_SHIFT+weights[i])));
    }
  }
}

void WeighedDetector::log() {
  Serial.print("(analog=[");
  for (int i = 0; i < 3; i++) {
    Serial.print(detections[i]);
    Serial.print(", ");
  }
  Serial.print("], digital=[");

  for (int i = 0; i < 3; i++) {
    Serial.print(digital_detections[i]);
    Serial.print(", ");
  }
  Serial.print("], weights=[");

  for (int i = 0; i < 3; i++) {
    Serial.print(weights[i]);
    Serial.print(", ");
  }
  Serial.print("], slope=");
  Serial.print(slope, 3);
  Serial.println(")");
}
