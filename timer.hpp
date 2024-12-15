//Arduino
/*
=================================================
Non-Blocking Timer Header
=================================================
*/


// =================== CLASS DEFINITION ===================
#ifndef NON_BLOCKING_TIMER_H
#define NON_BLOCKING_TIMER_H

#include <Arduino.h>

class NonBlockingTimer {
private:
    
    // Tracks the start time in milliseconds
    alignas(4) volatile unsigned long _start;

public:
    NonBlockingTimer();
    void start();
    unsigned long getElapsedTime();
    unsigned long getMsElapsedTime();
};

#endif
