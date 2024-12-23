//Arduino
/*
=================================================
SimpleRT - Module
=================================================
This module provides a simplified interface over FreeRTOS,
making task creation and periodic scheduling more intuitive.
Inspired by Python's asyncio, it allows developers to transfer
asynchronous programming concepts into microcontroller projects.

Main features:
  - Simplified task creation and management.
  - Periodic task execution using fixed time intervals.
  - Easy integration with FreeRTOS functions for multitasking.
*/

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "SimpleRT.hpp"

SimpleRT::SimpleRT(unsigned long period_ms) {
    // Initializes the SimpleRT instance with a periodic task interval.
    period = period_ms;
    xLastWakeTime = 0;
}

void SimpleRT::await(unsigned long time_ms) {
    // Pauses task execution for the specified time in milliseconds.
    TickType_t delayTicks = time_ms / portTICK_PERIOD_MS;
    vTaskDelay(delayTicks);
}

void SimpleRT::awaitNextIteration() {
    // Waits until the next iteration of a periodic task based on the defined
    // interval.
    xTaskDelayUntil(&xLastWakeTime, (period / portTICK_PERIOD_MS));
}

void SimpleRT::newTask(char* name, void (*func)(), int priority) {
    // Creates a new FreeRTOS task with a specified name, function, and priority.
    xTaskCreate(func, name, 128, NULL, priority, NULL);
}

void SimpleRT::start() {
    // Starts the FreeRTOS scheduler, enabling multitasking.
    vTaskStartScheduler();
}