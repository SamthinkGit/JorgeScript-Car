//Arduino
#include "FastLED.h"
#include "SimpleRT.hpp"

#define PIN_RBGLED 4
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

void setupLed() {
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
  }

void showColor(uint8_t r, uint8_t g, uint8_t b) {
  FastLED.showColor(Color(r, g, b));
}

void turnLightOff() {
  FastLED.showColor(Color(0, 0, 0)); 
}

void warningLights() {
  for (int i = 0; i < 3; i++) {
    showColor(255, 0, 0);
    delay(100);
    turnLightOff();
    delay(100);
  }
}

void aWarningLights() {
  SimpleRT rt = SimpleRT(0);
  for (int i = 0; i < 3; i++) {
    showColor(255, 0, 0);
    rt.await(100);
    turnLightOff();
    rt.await(100);
  }
}