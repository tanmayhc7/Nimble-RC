#include "unity.h"

#include "MockLCD_status.h"
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockdriver_logic.h"
#include "Mockgpio.h"

#include "can_mia_configurations.c"
#include "driver_node.h"
#include "project.h"

void test_driver_node__mia_handle_10hz(void) {
  gpio_s gpio = {};

  board_io__get_led2_ExpectAndReturn(gpio);
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  gpio__toggle_Expect(gpio);

  for (int i = 0; i < 11; i++) {
    driver_node__mia_handle_10hz();
  }
}

void test_driver_node__handle_sensor_messages_over_can(void) {
  can__msg_t can_msg = {0};
  dbc_SENSOR_DATA_s sensor_data = {0};

  can_msg.msg_id = dbc_header_SENSOR_DATA.message_id;
  can_msg.frame_fields.data_len = dbc_header_SENSOR_DATA.message_dlc;

  can__rx_ExpectAndReturn(can1, NULL, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);

  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();

  driver__process_sensor_input_Expect(&sensor_data);
  LCD_status__update_sensor_Expect(&sensor_data);
  driver_node__handle_messages_over_can();
}

void test_driver_node__handle_compass_messages_over_can(void) {
  can__msg_t can_msg = {0};
  dbc_COMPASS_s compass_data = {0};

  can_msg.msg_id = dbc_header_COMPASS.message_id;
  can_msg.frame_fields.data_len = dbc_header_COMPASS.message_dlc;

  can__rx_ExpectAndReturn(can1, NULL, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);

  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();

  driver__process_geo_controller_input_Expect(&compass_data);
  LCD_status__update_compass_Expect(&compass_data);
  driver_node__handle_messages_over_can();
}

void test_driver_node__handle_motor_data_messages_over_can(void) {
  can__msg_t can_msg = {0};
  dbc_MOTOR_DATA_s motor_data = {0};

  can_msg.msg_id = dbc_header_MOTOR_DATA.message_id;
  can_msg.frame_fields.data_len = dbc_header_MOTOR_DATA.message_dlc;

  can__rx_ExpectAndReturn(can1, NULL, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);

  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();

  LCD_status__update_motor_debug_Expect(&motor_data);
  driver_node__handle_messages_over_can();
}

void test_driver_node__transmit_motor_cmd_over_can(void) {
  dbc_MOTOR_CMD_s motor_cmd_data = {0};

  driver__get_motor_commands_ExpectAndReturn(motor_cmd_data);
  can__tx_ExpectAndReturn(can1, NULL, 0, true);
  can__tx_IgnoreArg_can_message_ptr();

  driver_node__transmit_motor_cmd_over_can();
}