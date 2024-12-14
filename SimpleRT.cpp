//Arduino
/*
=================================================
SimpleRT - Implementation
=================================================
*/
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "SimpleRT.hpp"

SimpleRT::SimpleRT(unsigned long period_ms) {
    period = period_ms;
    xLastWakeTime = 0;
}

void SimpleRT::await(unsigned long time_ms) {
    TickType_t delayTicks = time_ms / portTICK_PERIOD_MS;
    vTaskDelay(delayTicks);
}

void SimpleRT::awaitNextIteration() {
    xTaskDelayUntil(&xLastWakeTime, (period / portTICK_PERIOD_MS));
}

void SimpleRT::newTask(char* name, void (*func)(), int priority) {
    xTaskCreate(func, name, 128, NULL, priority, NULL);
}

void SimpleRT::start() {
    vTaskStartScheduler();
}