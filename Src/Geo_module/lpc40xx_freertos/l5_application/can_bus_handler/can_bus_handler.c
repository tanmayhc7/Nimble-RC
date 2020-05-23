#include "can_bus_handler.h"
#include "can_bus.h"
#include "geo_node.h"

void can_bus_handler__init(void) {
  can__init(can1, 100, 50, 50, (can_void_func_t)1, (can_void_func_t)1);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
}

void can_bus_handler_bus_off(void) {
  if (can__is_bus_off(can1)) {
    can__reset_bus(can1);
  }
}

void can_bus_handler__mia_handle_10Hz(void) { geo_node__mia_handle_10Hz(); }

void can_bus_handler__process_all_received_messages_10Hz(void) { geo_node__handle_can_messages_10Hz(); }

void can_bus_handler__transmit_messages_10Hz(void) { geo_node__transmit_can_messages_10Hz(); }
