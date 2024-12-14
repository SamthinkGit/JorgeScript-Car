/*
=================================================
SimpleRT - Header
=================================================
*/

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#ifndef SIMPLERT_HPP
#define SIMPLERT_HPP
class SimpleRT {

private:

  unsigned long period;
  TickType_t xLastWakeTime, aux;

public:
    SimpleRT(unsigned long period_ms);   // Here we stablish the frequency of the RT task
    void await(unsigned long time_ms);   // Await an specific set of ms
    void awaitNextIteration();
    static void newTask(char* name, void (*func)(), int priority);
    static void start();
};


#endif
