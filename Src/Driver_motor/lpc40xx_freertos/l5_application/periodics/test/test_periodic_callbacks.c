#include <stdio.h>
#include <string.h>

#include "unity.h"

#include "MockLCD_driver.h"
#include "MockLCD_status.h"
#include "Mockboard_io.h"
#include "Mockcan_bus_handler.h"
#include "Mockgpio.h"
#include "Mocki2c_initializer.h"

#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_bus_handler__init_Expect();
  i2c_initializer__lcd_init_Expect();
  LCD__reset_Expect();
  LCD__init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  can_bus_handler__handle_bus_off_Expect();
  LCD_status__display_handle_1hz_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  can_bus_handler__transmit_messages_Expect();
  can_bus_handler__transmit_driver_debug_can_init_Expect();
  can_bus_handler__mia_handle_10hz_Expect();
  periodic_callbacks__10Hz(0);
}

void test__periodic_callbacks__100Hz(void) {
  can_bus_handler__process_all_received_messages_Expect();
  periodic_callbacks__100Hz(4);
}