#include <Arduino_FreeRTOS.h>  // Include the RT lib
#include "SimpleRT.hpp"

// This is my rt function (The params will be passed as pointers)
void hello_world_v2(void *params) {

  SimpleRT rt = SimpleRT(500);

  while (true) {
    Serial.println("[2] Hello RTOS!");
    rt.awaitNextIteration();

  }
}

void simple_led_setup(void *params) {

  SimpleRT rt = SimpleRT(2000);

  while (true) {

    Serial.println("[1] ON");
    rt.await(1000);
    Serial.println("[1] OFF");
    rt.awaitNextIteration();
    

  }
}

void setup() {


  Serial.begin(9600);
  Serial.println("\n\n\n\n");
  Serial.println("----- STARTING -----");

  // We create the task for the RT Planner
  xTaskCreate(hello_world_v2, "task_1", 128, NULL, 1, NULL);
  xTaskCreate(simple_led_setup, "task_1", 128, NULL, 1, NULL);

  // Run the planner
  vTaskStartScheduler();
}

void loop() {
}