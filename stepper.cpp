#include <Arduino.h>
#include "stepper.hpp"

void setup_step() {
  pinMode(STK_MODE_PIN, OUTPUT);
  pinMode(STK_CLOCK_PIN, OUTPUT);
  pinMode(STK_CWB_PIN, OUTPUT);
  pinMode(STK_ENABLE_PIN, OUTPUT);

  digitalWrite(STK_MODE_PIN, 0);
  digitalWrite(STK_CLOCK_PIN, 0);
  digitalWrite(STK_CWB_PIN, 0);
  digitalWrite(STK_ENABLE_PIN, 0);
  delay(1);
  digitalWrite(STK_ENABLE_PIN, 1);

  digitalWrite(STK_MODE_PIN, 1);
  digitalWrite(STK_CLOCK_PIN, 0);
  digitalWrite(STK_CWB_PIN, 0);
}

void step_it() {
  static unsigned long next_time = 0;
  static unsigned long steps = 0;
  static unsigned int direction = 0;
  unsigned long current_time = millis();
  if(current_time < next_time) {
    return;
  }
  steps++;
  if(!(steps % 480)) {
    direction = 1-direction;
    digitalWrite(STK_CWB_PIN, direction);
  }
  next_time = current_time + 20;
  digitalWrite(STK_CLOCK_PIN,1);
  delayMicroseconds(20);
  digitalWrite(STK_CLOCK_PIN,0);
  // digitalWrite(STK_ENABLE_PIN, 0);
  // delayMicroseconds(20);
  // digitalWrite(STK_ENABLE_PIN, 1);

}
