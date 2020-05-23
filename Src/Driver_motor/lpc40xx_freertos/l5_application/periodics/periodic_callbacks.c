#include "periodic_callbacks.h"
#include <stdio.h>

#include "can_bus_handler.h"
#include "i2c_initializer.h"

#include "clock.h"
#include "gpio.h"
#include "i2c.h"

#include "LCD_driver.h"
#include "LCD_status.h"

void periodic_callbacks__initialize(void) {
  can_bus_handler__init();
  i2c_initializer__lcd_init();
  LCD__reset();
  LCD__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  can_bus_handler__handle_bus_off();
  LCD_status__display_handle_1hz(); // To do! need to change to 1Hz
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  can_bus_handler__transmit_messages();
  can_bus_handler__transmit_driver_debug_can_init();
  can_bus_handler__mia_handle_10hz();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {
  if (callback_count % 5 == 4) {
    can_bus_handler__process_all_received_messages();
  }
}

void periodic_callbacks__1000Hz(uint32_t callback_count) {}