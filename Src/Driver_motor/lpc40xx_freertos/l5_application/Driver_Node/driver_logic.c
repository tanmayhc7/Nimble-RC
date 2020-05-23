#include "driver_logic.h"
#include <stdint.h>

static dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {0};
static dbc_MOTOR_CMD_s driver_MOTOR_CMD = {0};
static dbc_COMPASS_s compass_heading = {0};

void driver__process_sensor_input(dbc_SENSOR_DATA_s *sensor_data) { driver_SENSOR_SONAR = *sensor_data; }

void driver__process_geo_controller_input(dbc_COMPASS_s *heading) { compass_heading = *heading; }

static void driver__update_motor_cmd(motor_steer_cmd_e motor_steer_cmd, motor_speed_cmd_e motor_speed) {
  driver_MOTOR_CMD.MOTOR_CMD_STEERING = motor_steer_cmd;
  driver_MOTOR_CMD.MOTOR_CMD_SPEED = motor_speed;
}

static float driver__logic_get_current_heading_compliment(void) {
  if (compass_heading.CMP_CURRENT_HEADING < 180) {
    return compass_heading.CMP_CURRENT_HEADING + 180;
  } else if (compass_heading.CMP_CURRENT_HEADING > 180) {
    return compass_heading.CMP_CURRENT_HEADING - 180;
  } else {
    return 0;
  }
}

static void driver_logic__determine_motor_commands_for_destination_heading(void) {

  float destination_bearing = compass_heading.CMP_DEST_BEARING;
  float current_heading = compass_heading.CMP_CURRENT_HEADING;
  float distance_to_destination = compass_heading.CMP_DISTANCE;
  float current_heading_compliment = driver__logic_get_current_heading_compliment();
  if (distance_to_destination <= 5.0) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_straight, MOTOR_SPEED_CMD_stop);
  } else {
    if (current_heading == destination_bearing) {
      driver__update_motor_cmd(MOTOR_STEER_CMD_straight, MOTOR_SPEED_CMD_forward);
    } else if (current_heading >= 180) {
      if (destination_bearing > current_heading || destination_bearing < current_heading_compliment) {
        driver__update_motor_cmd(MOTOR_STEER_CMD_slight_right, MOTOR_SPEED_CMD_forward);
      } else if (destination_bearing < current_heading || destination_bearing > current_heading_compliment) {
        driver__update_motor_cmd(MOTOR_STEER_CMD_slight_left, MOTOR_SPEED_CMD_forward);
      }
    } else if (current_heading < 180) {
      if (destination_bearing < current_heading || destination_bearing > current_heading_compliment) {
        driver__update_motor_cmd(MOTOR_STEER_CMD_slight_left, MOTOR_SPEED_CMD_forward);
      } else if (destination_bearing > current_heading || destination_bearing < current_heading_compliment) {
        driver__update_motor_cmd(MOTOR_STEER_CMD_slight_right, MOTOR_SPEED_CMD_forward);
      }
    }
  }
}

static void driver_logic__get_motor_commands_for_far_away_front_object(void) {

  if ((driver_SENSOR_SONAR.SENSOR_SONARS_right <= sensor_threshold_mid) &&
      (driver_SENSOR_SONAR.SENSOR_SONARS_left > sensor_threshold_mid)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_slight_left, MOTOR_SPEED_CMD_forward);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right > sensor_threshold_mid)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_slight_right, MOTOR_SPEED_CMD_forward);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_left > driver_SENSOR_SONAR.SENSOR_SONARS_right)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_slight_left, MOTOR_SPEED_CMD_forward);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right > driver_SENSOR_SONAR.SENSOR_SONARS_left)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_slight_right, MOTOR_SPEED_CMD_forward);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right == driver_SENSOR_SONAR.SENSOR_SONARS_left)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_straight, MOTOR_SPEED_CMD_forward);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left > sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right > sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_IR_rear > sensor_threshold_mid)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_straight, MOTOR_SPEED_CMD_forward);

    driver_logic__determine_motor_commands_for_destination_heading();
  }
}

static void driver_logic__get_motor_commands_for_near_front_object(void) {
  if ((driver_SENSOR_SONAR.SENSOR_SONARS_right <= sensor_threshold_mid) &&
      (driver_SENSOR_SONAR.SENSOR_SONARS_left > sensor_threshold_mid)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_left, MOTOR_SPEED_CMD_forward);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right > sensor_threshold_mid)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_right, MOTOR_SPEED_CMD_forward);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_IR_rear > sensor_threshold_near)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_straight, MOTOR_SPEED_CMD_reverse);

  } else if ((driver_SENSOR_SONAR.SENSOR_SONARS_left <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_SONARS_right <= sensor_threshold_mid) &&
             (driver_SENSOR_SONAR.SENSOR_IR_rear <= sensor_threshold_near)) {
    driver__update_motor_cmd(MOTOR_STEER_CMD_straight, MOTOR_SPEED_CMD_stop);
  }
}

dbc_MOTOR_CMD_s driver__get_motor_commands(void) {
  if (driver_SENSOR_SONAR.SENSOR_SONARS_mid > sensor_threshold_mid) {
    driver_logic__get_motor_commands_for_far_away_front_object();

  } else if (driver_SENSOR_SONAR.SENSOR_SONARS_mid <= sensor_threshold_mid) {
    driver_logic__get_motor_commands_for_near_front_object();
  }
  return driver_MOTOR_CMD;
}
