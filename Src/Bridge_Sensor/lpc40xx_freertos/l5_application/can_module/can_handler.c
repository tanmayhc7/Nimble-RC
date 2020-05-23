#include <stdio.h>

#include "can_bus.h"
#include "can_handler.h"
#include "project.h"

#include "bluetooth.h"
#include "buffer_handler.h"
/********************************************
 *          Function Prototypes
 *******************************************/
void buf__update_sensor_struct(dbc_SENSOR_DATA_s *encode_sensor_struct);

/********************************************
 *     CAN Initialize & Reset Functions
 *******************************************/

void can_bus_handler__init(void) {
  can__init(can1, 100, 50, 50, (can_void_func_t)1, (can_void_func_t)1);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
}

void can_bus_handler__handle_bus_off(void) {
  if (can__is_bus_off(can1)) {
    can__reset_bus(can1);
  }
}

/********************************************
 *       Transmit Functions & Helpers
 *******************************************/
void can_bus_handler__transmit_messages_100Hz(void) {
  dbc_SENSOR_DATA_s encode_sensor_struct = {0};
  buf__update_sensor_struct(&encode_sensor_struct);
  if (!dbc_encode_and_send_SENSOR_DATA(NULL, &encode_sensor_struct)) {
    puts("Message not sent over can\n");
  } else {
    puts("Message sent\n");
  }
}

void buf__update_sensor_struct(dbc_SENSOR_DATA_s *encode_sensor_struct) {
  encode_sensor_struct->SENSOR_SONARS_left = buf__return_min_value(2);
  encode_sensor_struct->SENSOR_SONARS_mid = buf__return_min_value(3);
  encode_sensor_struct->SENSOR_SONARS_right = buf__return_min_value(4);
  encode_sensor_struct->SENSOR_IR_rear = buf__return_min_value(5);
}

void can_handler__transmit_messages_1Hz(void) {
  dbc_GPS_DESTINATION_INFO_s encode_bridge_gps_struct = {};
  encode_bridge_gps_struct = bt__can_update_destination_gps_coordinates();
  return dbc_encode_and_send_GPS_DESTINATION_INFO(NULL, &encode_bridge_gps_struct);
}

bool dbc_send_can_message(void *argument_from_dbc_encode_and_send, uint32_t message_id, const uint8_t bytes[8],
                          uint8_t dlc) {
  (void)argument_from_dbc_encode_and_send;

  // Encode struct to bytes of the CAN message
  can__msg_t can_msg = {0};
  can_msg.msg_id = message_id;
  can_msg.frame_fields.data_len = dlc;

  memcpy(can_msg.data.bytes, bytes, dlc);
  return can__tx(can1, &can_msg, 0);
}
