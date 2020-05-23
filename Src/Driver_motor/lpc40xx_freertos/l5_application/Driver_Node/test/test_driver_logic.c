#include "unity.h"

#include "driver_logic.c"
#include "project.h"

static const int8_t steer_left = -2;
static const int8_t steer_right = 2;
static const int8_t steer_straight = 0;
static const int8_t steer_slight_left = -1;
static const int8_t steer_slight_right = 1;
static const int8_t speed_forward = 15;
static const int8_t speed_reverse = -5;

static const uint8_t obstacle_far = 80;
static const uint8_t obstacle_mid = 65;
static const uint8_t obstacle_near = 30;

void setUp(void) { dbc_MOTOR_CMD_s driver_MOTOR_CMD = {0}; }

void test_driver_logic__process_input(void) {
  dbc_SENSOR_DATA_s incoming_sensor_data = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = 14,
      .SENSOR_SONARS_right = 24,
      .SENSOR_SONARS_mid = 30,
      .SENSOR_IR_rear = 43,
  };

  driver__process_sensor_input(&incoming_sensor_data);
  TEST_ASSERT_EQUAL(driver_SENSOR_SONAR.SENSOR_SONARS_left, 14);
  TEST_ASSERT_EQUAL(driver_SENSOR_SONAR.SENSOR_SONARS_right, 24);
  TEST_ASSERT_EQUAL(driver_SENSOR_SONAR.SENSOR_SONARS_mid, 30);
  TEST_ASSERT_EQUAL(driver_SENSOR_SONAR.SENSOR_IR_rear, 43);
}

void test_driver__turn_slight_left_for_far_front_obstacle(void) {

  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_left);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__turn_slight_right_for_far_front_obstacle(void) {

  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_right);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__right_greater_than_left_sensor_below_threshold_turn_slight_right(void) {

  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_mid,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_right);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__left_greater_than_right_sensor_below_threshold_turn_slight_left(void) {

  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_mid,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_left);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__left_equal_to_right_sensor_below_threshold(void) {

  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_near,
  };

  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_straight);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__turn_left_for_near_front_obstacle(void) {

  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_near,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_left);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__turn_right_for_near_front_obstacle(void) {

  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_near,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_right);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__get_motor_commands_drive_forward(void) {
  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  compass_heading.CMP_DISTANCE = 30;

  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_STEERING, 0);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver__get_motor_commands_reverse(void) {
  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_near,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, 0);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_reverse);
}

void test_driver__get_motor_commands_stop(void) {
  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_near,
      .SENSOR_IR_rear = obstacle_near,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, 0);
  TEST_ASSERT_EQUAL(driver_MOTOR_CMD.MOTOR_CMD_SPEED, 0);
}

void test_driver_logic_get_motor_commands_for_current_heading_equal_destination_bearing(void) {
  compass_heading.CMP_CURRENT_HEADING = 180;
  compass_heading.CMP_DEST_BEARING = 180;
  driver_logic__determine_motor_commands_for_destination_heading();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, 0);
}

void test_driver_logic__get_motor_commands_for_current_heading_north(void) {

  compass_heading.CMP_CURRENT_HEADING = 0;
  compass_heading.CMP_DEST_BEARING = 323;
  compass_heading.CMP_DISTANCE = 30;

  driver_logic__determine_motor_commands_for_destination_heading();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_left);

  compass_heading.CMP_CURRENT_HEADING = 0;
  compass_heading.CMP_DEST_BEARING = 150;
  driver_logic__determine_motor_commands_for_destination_heading();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_right);
}

void test_driver_logic__get_motor_commands_for_compass_heading_south(void) {

  compass_heading.CMP_CURRENT_HEADING = 180;
  compass_heading.CMP_DEST_BEARING = 323;
  compass_heading.CMP_DISTANCE = 30;

  driver_logic__determine_motor_commands_for_destination_heading();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_right);

  compass_heading.CMP_CURRENT_HEADING = 180;
  compass_heading.CMP_DEST_BEARING = 150;
  driver_logic__determine_motor_commands_for_destination_heading();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_left);
}

void test_driver_logic__no_obstacles_detected_continue_destination_heading(void) {
  dbc_SENSOR_DATA_s driver_SENSOR_SONAR = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&driver_SENSOR_SONAR);
  compass_heading.CMP_CURRENT_HEADING = 0;
  compass_heading.CMP_DEST_BEARING = 170;
  compass_heading.CMP_DISTANCE = 30;
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_right);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver_logic__far_front_and_left_obstacle_detected_transition_from_navigation_to_obstacle_avoidance(void) {
  dbc_SENSOR_DATA_s all_sensors_above_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&all_sensors_above_threshold);
  compass_heading.CMP_CURRENT_HEADING = 0;
  compass_heading.CMP_DEST_BEARING = 120;
  compass_heading.CMP_DISTANCE = 30;
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_right);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);

  dbc_SENSOR_DATA_s left_sensor_below_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&left_sensor_below_threshold);
  compass_heading.CMP_CURRENT_HEADING = 60;
  compass_heading.CMP_DEST_BEARING = 60;
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_right);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver_logic__far_front_and_right_obstacle_detected_transition_from_navigation_to_obstacle_avoidance(void) {
  dbc_SENSOR_DATA_s all_sensors_above_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&all_sensors_above_threshold);
  compass_heading.CMP_CURRENT_HEADING = 0;
  compass_heading.CMP_DEST_BEARING = 250;
  compass_heading.CMP_DISTANCE = 30;

  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_left);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);

  dbc_SENSOR_DATA_s right_sensor_below_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&right_sensor_below_threshold);
  compass_heading.CMP_CURRENT_HEADING = 60;
  compass_heading.CMP_DEST_BEARING = 90;
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_slight_left);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver_logic__near_front_and_right_obstacle_detected_transition_from_navigation_to_obstacle_avoidance_to_turn_left(
    void) {
  dbc_SENSOR_DATA_s all_sensors_above_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&all_sensors_above_threshold);
  compass_heading.CMP_CURRENT_HEADING = 0;
  compass_heading.CMP_DEST_BEARING = 0;
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_straight);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);

  dbc_SENSOR_DATA_s right_sensor_below_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_near,
      .SENSOR_SONARS_mid = obstacle_near,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&right_sensor_below_threshold);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_left);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}

void test_driver_logic__near_front_and_left_obstacle_detected_transition_from_navigation_to_obstacle_avoidance_to_turn_right(
    void) {
  dbc_SENSOR_DATA_s all_sensors_above_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_far,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_far,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&all_sensors_above_threshold);
  compass_heading.CMP_CURRENT_HEADING = 0;
  compass_heading.CMP_DEST_BEARING = 0;
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_straight);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);

  dbc_SENSOR_DATA_s left_sensor_below_threshold = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = obstacle_near,
      .SENSOR_SONARS_right = obstacle_far,
      .SENSOR_SONARS_mid = obstacle_near,
      .SENSOR_IR_rear = obstacle_far,
  };
  driver__process_sensor_input(&left_sensor_below_threshold);
  driver_MOTOR_CMD = driver__get_motor_commands();
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_STEERING, steer_right);
  TEST_ASSERT_EQUAL_INT(driver_MOTOR_CMD.MOTOR_CMD_SPEED, speed_forward);
}
