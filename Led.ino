//Arduino
/*
=================================================
LED Wrapper for FastLED
=================================================
This module uses the FastLED library to control an RGB LED
connected to the microcontroller. It provides functions to
set colors, turn off the light, and display warning patterns.

Features:
  - Easy color control using RGB values.
  - Functions for turning the LED on and off.
  - Warning light patterns with blocking (`delay`) and 
    non-blocking (`SimpleRT`) approaches.
*/
#include "FastLED.h"
#include "SimpleRT.hpp"

#define PIN_RBGLED 4
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

void setupLed() {
  // Initializes the LED settings, including brightness.
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  // Converts RGB values into a 32-bit color for FastLED.
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
  }

void showColor(uint8_t r, uint8_t g, uint8_t b) {
  // Displays a solid color on the LED.
  FastLED.showColor(Color(r, g, b));
}

void turnLightOff() {
  // Turns off the LED by setting its color to black.
  FastLED.showColor(Color(0, 0, 0)); 
}

void warningLights() {
  // Displays a blocking warning pattern with the LED flashing red.
  for (int i = 0; i < 3; i++) {
    showColor(255, 0, 0);
    delay(100);
    turnLightOff();
    delay(100);
  }
}

void aWarningLights() {
  // Displays a non-blocking warning pattern using SimpleRT.
  SimpleRT rt = SimpleRT(0);
  for (int i = 0; i < 3; i++) {
    showColor(255, 0, 0);
    rt.await(100);
    turnLightOff();
    rt.await(100);
  }
}