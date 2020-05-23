#pragma once

#include "project.h"

typedef enum {
  MOTOR_STEER_CMD_left = -2,
  MOTOR_STEER_CMD_slight_left = -1,
  MOTOR_STEER_CMD_straight = 0,
  MOTOR_STEER_CMD_slight_right = 1,
  MOTOR_STEER_CMD_right = 2,
} motor_steer_cmd_e;

typedef enum {
  MOTOR_SPEED_CMD_reverse = -5,
  MOTOR_SPEED_CMD_stop = 0,
  MOTOR_SPEED_CMD_forward = 15,
} motor_speed_cmd_e;

typedef enum {
  sensor_threshold_near = 30,
  sensor_threshold_mid = 55,
  sensor_threshold_far = 80,

} sensor_threshold_e;

void driver__process_sensor_input(dbc_SENSOR_DATA_s *sensor_data);

void driver__process_geo_controller_input(dbc_COMPASS_s *heading);

dbc_MOTOR_CMD_s driver__get_motor_commands(void);
