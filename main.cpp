#include <Arduino.h>
#include <math.h>
#include "aJSON.h"
#include "DriveTrain.hpp"
#include "main.h"


extern "C" void __cxa_pure_virtual(void) {
    while(1);
}






void run_tests();
DriveTrain drive_train;;

void setup() {
  Serial.begin(115200);

  myPrintln("Startup...");

  analogReference(INTERNAL1V1);
}

int testvar = LOW;

void loop() {
  run_tests();
  drive_train.run();
}



int main(void)
{
  init();
  setup();
  for (;;){
    loop();
  }
  return 0;
}



void dt_run_slow_forward();
void dt_run_medium_forward();
void dt_run_full_forward();
void dt_run_slow_reverse();
void dt_run_medium_reverse();
void dt_run_full_reverse();
void dt_run_fast();
void dt_turn_right();
void dt_turn_left();
void dt_turn_hard_right();
void dt_turn_hard_left();
void dt_stop();
void dt_end();


unsigned long next_test_index = 0;
unsigned long next_test_time = 0;
int test_iterations = 0;
struct test_function {
  void (*fn)();
  unsigned long wait;
};

struct test_function tf[] = {
  {.fn = &dt_run_slow_forward,   .wait = 2000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_medium_forward, .wait = 2000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_slow_reverse,   .wait = 2000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_medium_reverse, .wait = 2000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_slow_forward,   .wait = 200 },
  {.fn = &dt_turn_right,         .wait = 2000 },
  {.fn = &dt_turn_left,          .wait = 2000 },
  {.fn = &dt_turn_right,         .wait = 2000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_full_forward,   .wait = 2000 },
  {.fn = &dt_turn_hard_left,     .wait = 2000 },
  {.fn = &dt_turn_hard_right,    .wait = 2000 },
  {.fn = &dt_end,                .wait = 100 },
  {.fn = NULL,                   .wait = 10 }
};



void run_tests() {
  unsigned long current_time = millis();
  if(next_test_time <= current_time) {
    int current_test_index = next_test_index++;
    next_test_time = current_time + tf[current_test_index].wait;
    Serial.print("Test Index ");
    Serial.print(current_test_index);
    Serial.print("(next test at  ");
    Serial.print(next_test_time);
    Serial.print(") : ");
    if(tf[current_test_index].fn) {
      tf[current_test_index].fn();
    } else {
      Serial.print("Iterations: ");
      Serial.print(" : ");
      Serial.println(++test_iterations);
      next_test_index = 0;
    }
  }
}



void dt_run_slow_forward() {
  myPrintln("drive_train.speed(.2)");
  drive_train.speed(.2);
}

void dt_run_medium_forward() {
  myPrintln("drive_train.speed(.5);");
  drive_train.speed(.5);
}

void dt_run_full_forward() {
  myPrintln("drive_train.speed(1);");
  drive_train.speed(1);
}

void dt_run_slow_reverse() {
  myPrintln("drive_train.speed(-.2)");
  drive_train.speed(-.2);
}

void dt_run_medium_reverse() {
  myPrintln("drive_train.speed(-.5);");
  drive_train.speed(-.5);
}
void dt_run_full_reverse() {
  myPrintln("drive_train.speed(1);");
  drive_train.speed(-1);
}

void dt_stop() {
  myPrintln("drive_train.stop()");
  drive_train.stop();
}

void dt_end() {
  myPrintln("********************** End of Test - looping back ***********************");
}

void dt_turn_right() {
  myPrintln("drive_train.turn(.5)");
  drive_train.turn(.5);
}

void dt_turn_hard_right() {
  myPrintln("drive_train.turn(1)");
  drive_train.turn(1);
}

void dt_turn_left() {
  myPrintln("drive_train.turn(-.5)");
  drive_train.turn(-.5);
}
void dt_turn_hard_left() {
  myPrintln("drive_train.turn(-1)");
  drive_train.turn(-1);
}
