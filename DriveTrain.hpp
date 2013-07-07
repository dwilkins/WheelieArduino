#ifndef DRIVETRAIN_HPP
#define DRIVETRAIN_HPP 1
#include "Wheel.hpp"

#define MAX_WHEELS 2

#ifndef speed_t
#define speed_t float
#endif

#define turn_t float


class DriveTrain {
public:
  DriveTrain();

  enum {LEFT_WHEEL, RIGHT_WHEEL} wheel;
  typedef enum {NONE, LEFT, RIGHT } direction_t;

  // Movement
  int turn(turn_t amount,  int seconds_duration = -1 );
  int speed(speed_t amount);
  int low_speed();
  int high_speed();
  int stop();
  // Processing
  int run();
  // Status
  const char *status();


protected:
  Wheel left_wheel;
  Wheel right_wheel;
  Wheel *wheels[MAX_WHEELS];
  turn_t current_turn;
  speed_t current_speed;
  speed_t left_speed;
  speed_t right_speed;

  DriveTrain::direction_t direction_from_turn(turn_t turn);


};

#endif
