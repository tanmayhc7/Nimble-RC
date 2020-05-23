#include "can_bus_handler.h"
#include "can_bus.h"

#include "driver_node.h"
#include "project.h"

static dbc_DRIVER_DEBUG_s driver_debug_s = {};

void can_bus_handler__init(void) {
  if (can__init(can1, 100, 50, 50, (can_void_func_t)1, (can_void_func_t)1)) {
    driver_debug_s.IO_DEBUG_CAN_init = 1;
  } else if (!(can__init(can1, 100, 50, 50, (can_void_func_t)1, (can_void_func_t)1))) {
    driver_debug_s.IO_DEBUG_CAN_init = 0;
  }
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
}

void can_bus_handler__handle_bus_off(void) {
  if (can__is_bus_off(can1)) {
    can__reset_bus(can1);
  }
}

void can_bus_handler__mia_handle_10hz(void) { driver_node__mia_handle_10hz(); }

void can_bus_handler__process_all_received_messages(void) { driver_node__handle_messages_over_can(); }

void can_bus_handler__transmit_messages(void) { driver_node__transmit_motor_cmd_over_can(); }

void can_bus_handler__transmit_driver_debug_can_init(void) { dbc_encode_and_send_DRIVER_DEBUG(NULL, &driver_debug_s); }
