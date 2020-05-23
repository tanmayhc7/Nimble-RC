#include "can_bus.h"
#include "project.h"

const uint32_t dbc_mia_threshold_SENSOR_DATA = 10 * 100;
const uint32_t dbc_mia_threshold_COMPASS = 10 * 100;
const uint32_t dbc_mia_threshold_MOTOR_DATA = 10 * 100;

const dbc_SENSOR_DATA_s dbc_mia_replacement_SENSOR_DATA = {};

const dbc_COMPASS_s dbc_mia_replacement_COMPASS = {};

const dbc_MOTOR_DATA_s dbc_mia_replacement_MOTOR_DATA = {};

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
