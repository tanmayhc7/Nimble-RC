#include <stdint.h>
#include <stdio.h>

#include "driver_node.h"

#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"

#include "LCD_status.h"
#include "driver_logic.h"
#include "project.h"

static dbc_SENSOR_DATA_s can_msg__sensor_sonars_status;
static dbc_COMPASS_s can_msg__compass_heading_status;
static dbc_MOTOR_DATA_s can_msg_motor_data_status;

void driver_node__mia_handle_10hz(void) {
  const uint32_t mia_increment_value_ms = 100;

  if ((dbc_service_mia_SENSOR_DATA(&can_msg__sensor_sonars_status, mia_increment_value_ms))) {
    gpio__toggle(board_io__get_led2());
  } else if ((dbc_service_mia_COMPASS(&can_msg__compass_heading_status, mia_increment_value_ms))) {
    gpio__toggle(board_io__get_led3());
  } else if ((dbc_service_mia_MOTOR_DATA(&can_msg_motor_data_status, mia_increment_value_ms))) {
    gpio__toggle(board_io__get_led1());
  }
}

void driver_node__handle_messages_over_can(void) {
  can__msg_t can_msg = {0};

  while (can__rx(can1, &can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };
    if (dbc_decode_SENSOR_DATA(&can_msg__sensor_sonars_status, header, can_msg.data.bytes)) {
      driver__process_sensor_input(&can_msg__sensor_sonars_status);
      LCD_status__update_sensor(&can_msg__sensor_sonars_status);
    } else if (dbc_decode_COMPASS(&can_msg__compass_heading_status, header, can_msg.data.bytes)) {
      driver__process_geo_controller_input(&can_msg__compass_heading_status);
      LCD_status__update_compass(&can_msg__compass_heading_status);
    } else if (dbc_decode_MOTOR_DATA(&can_msg_motor_data_status, header, can_msg.data.bytes)) {
      LCD_status__update_motor_debug(&can_msg_motor_data_status);
    }
  }
}

bool driver_node__transmit_motor_cmd_over_can(void) {
  dbc_MOTOR_CMD_s encode_motor_cmd_struct = {};
  encode_motor_cmd_struct = driver__get_motor_commands();
  return dbc_encode_and_send_MOTOR_CMD(NULL, &encode_motor_cmd_struct);
}
