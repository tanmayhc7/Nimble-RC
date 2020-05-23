#include <stdio.h>
#include <string.h>

#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_bus_handler.h"
#include "Mockcmps_button_app.h"
#include "Mockcompass.h"
#include "Mockgpio.h"
#include "Mockgps.h"

#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_bus_handler__init_Expect();
  gps__init_Expect();
  compass__init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  can_bus_handler_bus_off_Expect();
  cmps_button_app__periodic_1Hz_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  can_bus_handler__mia_handle_10Hz_Expect();
  can_bus_handler__process_all_received_messages_10Hz_Expect();
  can_bus_handler__transmit_messages_10Hz_Expect();
  gps__run_once_Expect();
  compass__run_once_8_bit_Expect();
  periodic_callbacks__10Hz(0);
}