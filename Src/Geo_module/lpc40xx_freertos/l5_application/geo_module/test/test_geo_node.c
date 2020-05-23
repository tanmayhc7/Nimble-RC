#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgeo_processor.h"
#include "Mockgpio.h"

#include "can_mia_configurations.c"
#include "geo_node.c"

void test_geo_node__mia_handle_10Hz(void) {
  gpio_s gpio = {};
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  for (int i = 0; i < 8; i++) {
    geo_node__mia_handle_10Hz();
  }
}

void test_geo_node__handle_can_messages_10Hz(void) {
  can__msg_t can_msg = {0};

  can_msg.msg_id = dbc_header_GPS_DESTINATION_INFO.message_id;
  can_msg.frame_fields.data_len = dbc_header_GPS_DESTINATION_INFO.message_dlc;

  can__rx_ExpectAndReturn(can1, NULL, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ReturnThruPtr_can_message_ptr(&can_msg);

  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  geo_processor__process_GPS_destination_Expect(&geo_gps_destination_status);

  geo_node__handle_can_messages_10Hz();
}

void test_geo_node__transmit_can_messages_10Hz(void) {
  dbc_COMPASS_s compass_heading_data = {0};
  geo_processor__get_compass_data_ExpectAndReturn(compass_heading_data);
  can__tx_ExpectAndReturn(can1, NULL, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  TEST_ASSERT_TRUE(geo_node__transmit_can_messages_10Hz());
}
