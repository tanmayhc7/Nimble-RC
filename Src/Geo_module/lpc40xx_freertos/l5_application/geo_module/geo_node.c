#include "geo_node.h"

#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"

#include "geo_processor.h"
#include "project.h"

static dbc_GPS_DESTINATION_INFO_s geo_gps_destination_status = {0};

void geo_node__mia_handle_10Hz(void) {
  const uint32_t mia_increment_value_ms = 100;

  if ((dbc_service_mia_GPS_DESTINATION_INFO(&geo_gps_destination_status, mia_increment_value_ms))) {
    gpio__toggle(board_io__get_led2());
  }
}

void geo_node__handle_can_messages_10Hz(void) {
  can__msg_t can_msg = {0};
  while (can__rx(can1, &can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };
    if (dbc_decode_GPS_DESTINATION_INFO(&geo_gps_destination_status, header, can_msg.data.bytes)) {
      geo_processor__process_GPS_destination(&geo_gps_destination_status);
    }
  }
}

bool geo_node__transmit_can_messages_10Hz(void) {
  dbc_COMPASS_s encode_compass_data_struct = geo_processor__get_compass_data();
  return dbc_encode_and_send_COMPASS(NULL, &encode_compass_data_struct);
}