#include "unity.h"

#include "Mockcan_bus.h"
#include "Mockdriver_logic.h"
#include "Mockdriver_node.h"

#include "can_bus_handler.c"
#include "can_mia_configurations.c"
#include "project.h"

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

void test_can_bus_handler__mia_handler_10hz(void) {
  driver_node__mia_handle_10hz_Expect();
  can_bus_handler__mia_handle_10hz();
}

void test_can_bus_handler__process_all_messages(void) {

  driver_node__handle_messages_over_can_Expect();
  can_bus_handler__process_all_received_messages();
}

void test_can_bus_handler__handle_transmit(void) {
  driver_node__transmit_motor_cmd_over_can_ExpectAndReturn(true);
  can_bus_handler__transmit_messages();
}

void test_can_bus_handler__transmit_driver_debug_can_init(void) {

  can__tx_ExpectAndReturn(can1, NULL, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  can_bus_handler__transmit_driver_debug_can_init();
  TEST_ASSERT_EQUAL(driver_debug_s.IO_DEBUG_CAN_init, 1);
}