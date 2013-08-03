#include <Arduino.h>
#include "Wheel.hpp"
#include "DriveTrain.hpp"

DriveTrain::DriveTrain ()
  : left_wheel(5,52),
    right_wheel(7,50) {
  wheels[LEFT_WHEEL] = &left_wheel;
  wheels[RIGHT_WHEEL] = &right_wheel;
  current_turn = 0;
  current_speed = 0;
  low_speed();
}


int DriveTrain::turn(turn_t amount,int seconds_duration ) {
  float speed_adjustment = 0.0;
  DriveTrain::direction_t direction = direction_from_turn(amount);
  left_speed = current_speed;
  right_speed = current_speed;
  if(current_speed != 0) {
    if(direction == LEFT) {
      speed_adjustment = 1.0 + amount;
      left_speed = current_speed * speed_adjustment;
    } else if(direction == RIGHT) {
      speed_adjustment = 1.0 - amount;
      right_speed = current_speed * speed_adjustment;
    }
  }
  wheels[LEFT_WHEEL]->set_speed(left_speed);
  wheels[RIGHT_WHEEL]->set_speed(right_speed);
  return 0;
}

int DriveTrain::speed(speed_t amount = -10) {
  if((amount >= -1) && (amount <= 1)) {
    current_speed = amount;
  }
  turn(current_turn);
  return 0;
}

int DriveTrain::stop() {
  speed(0);
  return 0;
}

int DriveTrain::run() {
  wheels[LEFT_WHEEL]->run();
  wheels[RIGHT_WHEEL]->run();
 return 0;
}

const char * DriveTrain::status() {
  const char *status_json = "";
  return status_json;
}

int DriveTrain::low_speed() {
  wheels[LEFT_WHEEL]->set_pwm_max(WHEEL_PWM_MAX / 2);
  wheels[RIGHT_WHEEL]->set_pwm_max(WHEEL_PWM_MAX / 2);
  speed();
  return 0;
}

int DriveTrain::high_speed() {
  wheels[LEFT_WHEEL]->set_pwm_max(WHEEL_PWM_MAX);
  wheels[RIGHT_WHEEL]->set_pwm_max(WHEEL_PWM_MAX);
  speed();
  return 0;
}


DriveTrain::direction_t DriveTrain::direction_from_turn(turn_t turn) {
  DriveTrain::direction_t dir;
  dir = turn > 0 ? RIGHT : LEFT;
  dir = turn == 0 ? NONE : dir;
  return dir;
}
