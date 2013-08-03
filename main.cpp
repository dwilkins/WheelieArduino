#include <Arduino.h>
#include <math.h>
#include "aJSON.h"
#include "DriveTrain.hpp"
#include "stepper.hpp"
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
  setup_step();
}

int testvar = LOW;

void loop() {
  run_tests();
  drive_train.run();
  step_it();
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


/*
 * ******************** Test Functions Start ********************
 */

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

void dt_go_straight() {
  myPrintln("drive_train.turn(0)");
  drive_train.turn(0);
}

void dt_toggle_speed() {
  static int current_speed = 0;
  if(current_speed) {
    myPrintln("drive_train.toggle_speed(low_speed)");
    drive_train.low_speed();
    current_speed = 0;
  } else {
    myPrintln("drive_train.toggle_speed(high_speed)");
    drive_train.high_speed();
    current_speed = 1;
  }
}


/*
 * ************************************************************
 * ******************** Test Functions End ********************
 * ************************************************************
 */



struct test_function {
  void (*fn)();
  unsigned long wait;
};

struct test_function tf[] = {
  {.fn = &dt_run_slow_forward,   .wait = 2000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_medium_forward, .wait = 2000 },
  {.fn = &dt_stop,               .wait = 1000 },
  {.fn = &dt_run_slow_reverse,   .wait = 2000 },
  {.fn = &dt_stop,               .wait = 1000 },
  {.fn = &dt_run_medium_reverse, .wait = 2000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_slow_forward,   .wait = 200  },
  {.fn = &dt_turn_right,         .wait = 1000 },
  {.fn = &dt_turn_left,          .wait = 1000 },
  {.fn = &dt_turn_right,         .wait = 1000 },
  {.fn = &dt_turn_left,          .wait = 1000 },
  {.fn = &dt_stop,               .wait = 2000 },
  {.fn = &dt_run_full_forward,   .wait = 2000 },
  {.fn = &dt_turn_hard_left,     .wait = 2000 },
  {.fn = &dt_turn_hard_right,    .wait = 2000 },
  {.fn = &dt_go_straight,        .wait = 200  },
  {.fn = &dt_run_full_forward,   .wait = 200  },
//  {.fn = &dt_toggle_speed,       .wait = 2000 },
  {.fn = &dt_end,                .wait = 100  },
  {.fn = NULL,                   .wait = 200  }
};


template <typename T, size_t N>
inline
size_t SizeOfArray( const T(&)[ N ] )
{
  return N;
}

void run_tests() {
  static unsigned long next_test_index = 0;
  static unsigned long next_test_time = 0;
  static int test_iterations = 0;
  unsigned long current_time = millis();

  if(next_test_time <= current_time) {
    int current_test_index = next_test_index++;
    if(current_test_index > SizeOfArray(tf)) {
      next_test_index = 0;
      next_test_time = millis() + 100;
      return;
    }
    next_test_time = current_time + tf[current_test_index].wait;
    Serial.print("Test Number ");
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

