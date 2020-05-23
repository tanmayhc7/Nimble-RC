#include "unity.h"

#include "Mockbluetooth.h"
#include "Mockbuffer_handler.h"
#include "Mockcan_bus.h"

#include "can_handler.h"

void test_can_bus_handler__init(void) {
  can__init_ExpectAndReturn(can1, 100, 50, 50, (void *)1, (void *)1, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  can_bus_handler__init();
}

void test_can_bus_handler__handle_bus_off(void) {
  can__is_bus_off_ExpectAndReturn(can1, true);
  can__reset_bus_Expect(can1);
  can_bus_handler__handle_bus_off();
}

void test_can_bus_handler__handle_transmit(void) {
  can__msg_t can_msg;
  dbc_SENSOR_DATA_s encode_sensor_struct = {
      .SENSOR_SONARS_left = 0, .SENSOR_SONARS_mid = 0, .SENSOR_SONARS_right = 0, .SENSOR_IR_rear = 0};
  buf__return_min_value_ExpectAndReturn(2, encode_sensor_struct.SENSOR_SONARS_left);
  buf__return_min_value_ExpectAndReturn(3, encode_sensor_struct.SENSOR_SONARS_mid);
  buf__return_min_value_ExpectAndReturn(4, encode_sensor_struct.SENSOR_SONARS_right);
  buf__return_min_value_ExpectAndReturn(5, encode_sensor_struct.SENSOR_IR_rear);
  can__tx_ExpectAndReturn(can1, &can_msg, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  can_bus_handler__transmit_messages_100Hz();
}

void test_can_handler__transmit_messages_1Hz(void) {
  can__msg_t can_msg;
  dbc_GPS_DESTINATION_INFO_s dest_gps_info_encode_s;
  can__tx_ExpectAndReturn(can1, &can_msg, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  bt__can_update_destination_gps_coordinates_ExpectAndReturn(dest_gps_info_encode_s);
  can_handler__transmit_messages_1Hz();
}
