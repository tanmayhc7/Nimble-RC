#include "unity.h"

#include "Mockcan_bus.h"
#include "Mockgeo_node.h"

#include "can_bus_handler.h"
#include "project.h"

void test_can_bus_handler__init(void) {
  can__init_ExpectAndReturn(can1, 100, 50, 50, (void *)1, (void *)1, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  can_bus_handler__init();
}

void test_can_bus_handler_bus_off(void) {
  can__is_bus_off_ExpectAndReturn(can1, true);
  can__reset_bus_Expect(can1);
  can_bus_handler_bus_off();
}

void test_can_bus_handler__mia_handle_10Hz(void) {
  geo_node__mia_handle_10Hz_Expect();
  can_bus_handler__mia_handle_10Hz();
}

void test_can_bus_handler__process_all_received_messages_10Hz(void) {
  geo_node__handle_can_messages_10Hz_Expect();
  can_bus_handler__process_all_received_messages_10Hz();
}

void test_can_bus_handler__transmit_messages_10Hz(void) {
  geo_node__transmit_can_messages_10Hz_ExpectAndReturn(true);
  can_bus_handler__transmit_messages_10Hz();
}
