//Arduino
#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0


void setupDetection() {
    pinMode(PIN_ITR20001_LEFT, INPUT);
    pinMode(PIN_ITR20001_MIDDLE, INPUT);
    pinMode(PIN_ITR20001_RIGHT, INPUT);
}

int* read() {
    static int values[3];
    values[0] = analogRead(PIN_ITR20001_LEFT);
    values[1] = analogRead(PIN_ITR20001_MIDDLE);
    values[2] = analogRead(PIN_ITR20001_RIGHT);
    return values;
}

void printValues(int* reading) {
  Serial.print("Reading(")
  for (int i = 0; i < 3; i++) {
    Serial.print(reading[i]);
    Serial.print(", ");
  }
  Serial.println(")");
}