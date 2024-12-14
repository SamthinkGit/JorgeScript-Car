/*
=================================================
Non-Blocking Timer Module
=================================================
This module provides a non-blocking timer for managing
time intervals without halting program execution. It is
useful for multitasking and asynchronous operations.

PD: There are a lot of time-based operations, so building
a clock takes away several headaches.
*/


// =================== CLASS IMPLEMENTATION ===================
#include "timer.hpp"

NonBlockingTimer::NonBlockingTimer() : _start(0) {}
void NonBlockingTimer::start() {
  // Starts the timer by recording the current system time in milliseconds.
    unsigned long currentMillis = millis();
    _start = currentMillis;    
}

unsigned long NonBlockingTimer::getElapsedTime() {
    // Calculates the elapsed time in [seconds] since the timer was started.
    unsigned long now, start;
    
    noInterrupts();
    now = millis();
    start = _start;
    interrupts();

    unsigned long elapsed = (now - start) / 1000;

    return elapsed;
    
}

unsigned long NonBlockingTimer::getMsElapsedTime() {
    // Calculates the elapsed time in [milliseconds] since the timer was started.
    unsigned long now, start;

    noInterrupts();
    now = millis();
    start = _start;
    interrupts();

    return now - start;
}
