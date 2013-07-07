#ifndef WHEEL_HPP
#define WHEEL_HPP 1

#define WHEEL_PWM_MAX (int)(255 * .8)
#define WHEEL_PWM_MIN 0
#define WHEEL_PWM_MAX_ADJUSTMENT 2
#define WHEEL_DEFAULT_RAMP_MILLISECONDS 10
#define WHEEL_DEFAULT_WORKER_MILLISECONDS 50


#define speed_t float

#define WHEEL_SPEED_MIN (speed_t)-1.0
#define WHEEL_SPEED_MAX (speed_t)1.0

class Wheel {
public:
  Wheel(int pwm_pin = 0,
        int dir_pin = 0,
        int brake_pin = 0,
        int pwm_max = WHEEL_PWM_MAX,
        int pwm_min = WHEEL_PWM_MIN,
        int pwm_max_adjustment = WHEEL_PWM_MAX_ADJUSTMENT);

  int set_speed(speed_t speed = 0.0);
  speed_t get_speed();

  typedef enum {NONE, FORWARD, REVERSE } direction_t;

  int set_pwm_min(int pwm_min = WHEEL_PWM_MIN);
  int set_pwm_max(int pwm_max = WHEEL_PWM_MAX);
  int set_pwm_max_adjustment(int pwm_max_adjustment = WHEEL_PWM_MAX_ADJUSTMENT);
  int set_dir_pin(int pin = 0);
  int set_pwm_pin(int pin = 0);
  int set_ramp_milliseconds_per_pwm(int milliseconds = WHEEL_DEFAULT_RAMP_MILLISECONDS);
  int get_current_direction();
  int get_target_direction();
  int get_previous_direction();

  int run();

protected:
  int dir_pin;
  int brake_pin;
  int pwm_pin;
  int current_pwm;
  int target_pwm;
  speed_t current_speed;
  speed_t target_speed;
  int ramp_millseconds_per_pwm;
  direction_t current_direction;
  direction_t target_direction;
  direction_t previous_direction;
  long last_activity_time;
  long next_activity_time;
  long current_activity_time;
  long pwm_max_adjustment;
  long pwm_min;
  long pwm_max;
  bool brake;
  speed_t speed;
  bool reached_pwm_target;
  bool reached_speed_target;



protected:
  int new_target();
  int apply_brake();
  int apply_pwm();
  int apply_direction();
  int release_brake();
  int change_pwm();
  int change_pwm_target(speed_t speed);
  int change_direction_target(Wheel::direction_t new_dir);
  int change_speed_target(speed_t speed);
  int new_speed_target(speed_t speed);
  int process_speed();
  int process_pwm();
  Wheel::direction_t direction_from_speed(speed_t speed);
  speed_t abs_speed(speed_t speed);
  int set_next_activity_time(unsigned long base = 0, unsigned long delta = 0);
};

#endif
