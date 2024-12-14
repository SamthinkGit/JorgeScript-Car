#include "SimpleRT.hpp"
#include "timer.hpp"

NonBlockingTimer global_timer = NonBlockingTimer();

void setup() {

  Serial.begin(9600);
  Serial.println("----- STARTING -----");
  setupWheels();
  SimpleRT::newTask("aLoop", aLoop, 1);
  
  global_timer.start();
  SimpleRT::start();


}

void aLoop(void *params) {
  SimpleRT rt = SimpleRT(200);


  while(true) {
    if (global_timer.getElapsedTime() > 2) {
        setMotorSpeeds(0, 0);
        rt.awaitNextIteration();
        continue;
    }

    setMotorSpeedsFromSlope(0.8);
    rt.awaitNextIteration();
    Serial.println(global_timer.getElapsedTime());
  }

  
}


void loop() {
}
