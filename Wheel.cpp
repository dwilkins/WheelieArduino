#include <Arduino.h>
#include "Wheel.hpp"

#ifndef EINVAL
#define  EINVAL    22  /* Invalid argument */
#endif


Wheel::Wheel(int pwm_pin,
             int dir_pin,
             int brake_pin,
             int pwm_max,
             int pwm_min,
             int pwm_max_adjustment) {

  set_pwm_pin(pwm_pin);
  set_dir_pin(dir_pin);
  set_pwm_max(pwm_max);
  set_pwm_min(pwm_min);
  set_pwm_max_adjustment(pwm_max_adjustment);

  current_direction =  target_direction = previous_direction = NONE;
  last_activity_time = 0;
  next_activity_time = 0;
//  set_speed();
  set_ramp_milliseconds_per_pwm();
  reached_pwm_target = true;
  reached_speed_target = true;

}

/*
 * ********************************************************
 * ******************** Public Methods ********************
 * ********************************************************
 */



int Wheel::run() {
  int retval = -EINVAL;
  long current_time = millis();
  // Serial.print("current_time = ");
  // Serial.print(current_time,DEC);
  // Serial.print("   next_activity_time = ");
  // Serial.println(next_activity_time,DEC);
  if(next_activity_time > current_time) {
    return retval;
  }
  set_next_activity_time(current_time, WHEEL_DEFAULT_WORKER_MILLISECONDS);
  current_activity_time = current_time;
  if(!reached_speed_target && (retval = process_speed())) {
    reached_speed_target = true;
  } else {
    // Serial.print("reached_speed_target = ");
    // Serial.print(reached_speed_target,DEC);
    // Serial.print("   retval = ");
    // Serial.println(retval,DEC);
  }
  last_activity_time = current_activity_time;
  return retval;
}

int Wheel::set_pwm_pin(int pin) {
  int retval = -EINVAL;
  this->pwm_pin = pin;
  if(pwm_pin > 0) {
    pinMode(pwm_pin,OUTPUT);
    digitalWrite(pwm_pin,LOW);
    analogWrite(this->pwm_pin,0);
  }
  return retval;
}

int Wheel::set_dir_pin(int pin) {
  int retval = -EINVAL;
  dir_pin = pin;
  if(dir_pin > 0) {
    pinMode(dir_pin,OUTPUT);
    digitalWrite(dir_pin,LOW);
    retval = 0;
  }
  return retval;
}

int Wheel::set_pwm_min(int pwm) {
  int retval = -EINVAL;
  if(pwm_min >= 0 && pwm_min < 256) {
    pwm_min = pwm;
    retval = 0;
  }
  return retval;
}

int Wheel::set_pwm_max(int pwm) {
  int retval = -EINVAL;
  if(pwm >= 0 && pwm < 256) {
    pwm_max = pwm;
    retval = 0;
  }
  return retval;
}

int Wheel::set_pwm_max_adjustment(int pwm) {
  int retval = -EINVAL;
  if(pwm >= WHEEL_PWM_MIN && pwm < WHEEL_PWM_MAX) {
    pwm_max_adjustment = pwm;
    retval = 0;
  }
  return retval;
}

/*
 * set_speed(speed_t speed)
 * positive speed is forward, negative is backward
 * -1 <= speed <= 1
 */
int Wheel::set_speed(speed_t speed) {
  int retval = -EINVAL;
  // Serial.print("set_speed speed = ");
  // Serial.println(speed);
  // Serial.print(__PRETTY_FUNCTION__);
  if(WHEEL_SPEED_MIN <= speed && WHEEL_SPEED_MAX >= speed) {
    retval = new_speed_target(speed);
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}


speed_t Wheel::get_speed() {
  current_speed = (speed_t)((pwm_max / current_pwm) * (current_direction == REVERSE ? -1 : 1));
  return current_speed;
}

int Wheel::set_ramp_milliseconds_per_pwm(int milliseconds) {
  ramp_millseconds_per_pwm = milliseconds;
  return 0;
}



/*
 * ***********************************************************
 * ******************** Protected Methods ********************
 * ***********************************************************
 */



/*
 * process_speed()
 * Check to see if the target speed has been achieved, and if not adjust direction and
 * pwm as appropriate
 */

int Wheel::process_speed() {
  int retval = -EINVAL;
  // Serial.print(__PRETTY_FUNCTION__);
  if(get_speed() == target_speed) {
    return retval;
  }
  retval = process_pwm();
  /*
   * get_speed() below computes speed based on current_pwm.
   */
  if(retval && get_speed() != target_speed ) {
    if((retval = change_direction_target(direction_from_speed(target_speed)))) {
      retval = change_pwm_target(target_speed);
    }
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

/*
 * process_pwm()
 * Check to see if the target pwm has been achieved.   Adjust pwm appropriately if not
 * returns -EINVAL if no change is appropriate
 */

int Wheel::process_pwm() {
  int retval = -EINVAL;
  // Serial.print(__PRETTY_FUNCTION__);
  if(target_pwm != current_pwm) {
    int old_pwm = current_pwm;
    if(target_pwm < current_pwm) {
      current_pwm = (current_pwm - pwm_max_adjustment < target_pwm ? target_pwm : current_pwm - pwm_max_adjustment);
    } else if(target_pwm > current_pwm) {
      current_pwm = (current_pwm + pwm_max_adjustment > target_pwm ? target_pwm : current_pwm + pwm_max_adjustment);
    }
    retval = 0;
    apply_pwm();
    set_next_activity_time(current_activity_time,WHEEL_DEFAULT_RAMP_MILLISECONDS);
  } else {
    reached_pwm_target = true;
    if(current_pwm == 0 &&
       target_direction != current_direction &&
       direction_from_speed(target_speed) != current_direction) {
      change_direction_target(direction_from_speed(target_speed));
    }
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

int Wheel::apply_brake() {
  int retval = -EINVAL;
  // Serial.print(__PRETTY_FUNCTION__);
  if(!brake) {
    set_speed(0);
    brake = true;
    retval = 0;
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

int Wheel::release_brake() {
  int retval = -EINVAL;
  // Serial.print(__PRETTY_FUNCTION__);
  if(brake) {
    brake = false;
    retval = 0;
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

/*
 * new_speed_target(speed)
 * sets a new target speed for the wheel and does the right thing with direction
 * and pwm.   If direction changes, then change_direction_target(...) below will
 * set the pwm_target to 0.   If the direction doesn't change from what the current
 * direction is, then this method will call change_pwm_target to set the target_pwm
 * to the right thing
 */

int Wheel::new_speed_target(speed_t speed) {
  int retval = -EINVAL;
  // Serial.print(__PRETTY_FUNCTION__);
  Wheel::direction_t new_dir = direction_from_speed(speed);
  if(speed != target_speed || new_dir != target_direction) {
    reached_speed_target = false;
    target_speed = speed;
    if(new_dir != target_direction) {
      target_direction = new_dir;
      if((retval = change_direction_target(new_dir))) {
        retval = change_pwm_target(target_speed);
      }
    } else {
      retval = change_pwm_target(target_speed);
    }
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

/*
 * change_direction_target
 * if the way the wheel is spinning needs to change, the wheel needs to ramp down to 0
 * then ramp up again.   Otherwise there will be high amps while the Momentum is pushing
 * current back through the controller.   Slowing it down will avoid that.  If the
 * direction changes, this method will set pwm_target to 0 for the ramp down;
 */
int Wheel::change_direction_target(Wheel::direction_t new_dir) {
  int retval = -EINVAL;
  if(new_dir != target_direction) {
    retval = 0;
    if(current_direction == target_direction) {
      if(current_pwm > WHEEL_PWM_MIN) {
        target_direction = new_dir;
        change_pwm_target(0);
      } else {
        current_direction = target_direction = new_dir;
        apply_direction();
      }
    }
  } else {
    if(current_direction != target_direction) {
      if(current_pwm > WHEEL_PWM_MIN) {
        target_direction = new_dir;
        change_pwm_target(0);
      } else {
        current_direction = target_direction = new_dir;
        apply_direction();
      }
    }
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

/*
 * change_pwm_target
 * pwm is only concerned with the ABS of the speed - forward is positive and reverse is negative.
 * Either way, the pwm is the same.  There's a direction pin that sets which way the motor will
 * turn, and the motor needs to be still before that is changed....
 */

int Wheel::change_pwm_target(speed_t speed) {
  int retval = -EINVAL;
  float speed_percentage = 0;
  int new_target_pwm = 0;
  // Serial.print("change_pwm_target speed = ");
  // Serial.println(speed);

  speed = abs_speed(speed);
  // Serial.print("change_pwm_target abs_speed = ");
  // Serial.println(speed);
  // Serial.print(__PRETTY_FUNCTION__);
  if(speed > WHEEL_SPEED_MAX) {
    // Serial.print(" = ");
    // Serial.println(retval,DEC);
    return retval;
  }
  speed_percentage = speed / WHEEL_SPEED_MAX;
  new_target_pwm = pwm_max * speed_percentage;
  // Serial.print("(new_target_pwm  = ");
  // Serial.print(new_target_pwm);
  // Serial.print(")");
  if(new_target_pwm != target_pwm && new_target_pwm != current_pwm) {
    target_pwm = new_target_pwm;
    reached_pwm_target = false;
    retval = 0;
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

speed_t Wheel::abs_speed(speed_t speed) {
  return fabs(speed);
}

Wheel::direction_t Wheel::direction_from_speed(speed_t speed) {
  Wheel::direction_t dir;
  dir = speed > 0 ? FORWARD : REVERSE;
  dir = speed == 0 ? NONE : dir;
  return dir;
}

int Wheel::apply_pwm() {
  int retval = -EINVAL;
  // Serial.print(__PRETTY_FUNCTION__);
  if(pwm_pin > 0) {
    retval = 0;
    // Serial.print("Pin = ");
    // Serial.print(pwm_pin,DEC);
    // Serial.print("   target_pwm = ");
    // Serial.print(target_pwm);
    // Serial.print("   current = ");
    // Serial.println(current_pwm);
    analogWrite(pwm_pin, current_pwm);
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}

int Wheel::apply_direction() {
  int retval = -EINVAL;
  int dir_val = 0;
  Serial.print(__PRETTY_FUNCTION__);
  if(dir_pin > 0) {
    if(current_direction == REVERSE) {
      dir_val = 1;
    }
    digitalWrite(dir_pin,dir_val);
    retval = 0;
  }
  Serial.print("(dir_val = ");
  Serial.print(dir_val);
  Serial.print(") = ");
  Serial.println(retval,DEC);
  return retval;
}

int Wheel::set_next_activity_time(unsigned long delta,unsigned long base) {
  unsigned long current_time = millis();
  unsigned long nat = 0;
  int retval = -EINVAL;
  if(base < 1) {
    base = current_time;
  }
  if(delta < 0) {
    delta = WHEEL_DEFAULT_WORKER_MILLISECONDS;
  }
  nat = base + delta;
  if(next_activity_time <= current_time || next_activity_time >= nat) {
    retval = 0;
    this->next_activity_time = nat;
    // Serial.print(__PRETTY_FUNCTION__);
  }
  // Serial.print(" = ");
  // Serial.println(retval,DEC);
  return retval;
}
