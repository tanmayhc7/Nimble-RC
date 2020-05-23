#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "can_bus_handler.h"
#include "cmps_button_app.h"
#include "compass.h"
#include "gps.h"

void periodic_callbacks__initialize(void) {
  can_bus_handler__init();
  gps__init();
  compass__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led0());
  can_bus_handler_bus_off();
  cmps_button_app__periodic_1Hz();
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  can_bus_handler__mia_handle_10Hz();
  can_bus_handler__process_all_received_messages_10Hz();
  gps__run_once();
  compass__run_once_8_bit();
  can_bus_handler__transmit_messages_10Hz();
}
void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led3());
  // Add your code here
}