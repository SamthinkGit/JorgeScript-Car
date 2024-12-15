// Arduino 
/*
=================================================
Ultrasonic Distance Measurement Module
=================================================
This module provides functionality for measuring
distance using an ultrasonic sensor. It updates the
distance value only if the change exceeds a specified
threshold to minimize unnecessary updates.
*/

// =================== CONSTANTS AND CONFIGURATION ===================
const int TRIGGER_PIN = 13;             // Pin connected to the ultrasonic sensor's trigger.
const int ECHO_PIN = 12;                 // Pin connected to the ultrasonic sensor's echo.
const float SOUND_VEL = 34000.0;        // Speed of sound in air (in cm/s).
const float UPDATE_ONLY_OVER = 1;       // Minimum distance change required to trigger an update.
const float MIN_DISTANCE = 1;
const int MAX_WAITING_TIME = 4000;
const unsigned long MINIMUM_ELAPSED_SOUND_TIME = 200; // Min time between distance measurements (ms).

volatile float* _update_distance_value;             // Pointer to the variable that stores the measured distance.

// =================== FUNCTIONS ===================
void setupSound(float* update_value) {
   /* Initializes the ultrasonic sensor for measuring distance.
   *
   * - update_value: Pointer to a float variable where the distance
   *                 measurements will be stored.
   */
  _update_distance_value = update_value;
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  *_update_distance_value = 10000;
}

void updateDistance() {
  /* Measures the distance using the ultrasonic sensor and updates
   * the value if the change exceeds a specified threshold.
   */

    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, MAX_WAITING_TIME);
    float distance = duration * 0.000001 * SOUND_VEL / 2.0;
    if (distance < MIN_DISTANCE) {
      return;
    }

    float change = *_update_distance_value - distance;
    float abs_change = (change < 0) ? -change : change;

    if (abs_change > UPDATE_ONLY_OVER) {
      *_update_distance_value = distance;
    }
}
