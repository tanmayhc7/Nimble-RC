#include "periodic_callbacks.h"

#include "bluetooth.h"
#include "board_io.h"
#include "buffer_handler.h"
#include "can_handler.h"
#include "gpio.h"
#include "sensor_get_value.h"
#include "sensor_initializer.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  sensor_node__init_all_sensors();
  sensor_node__init_trigger_pins_as_gpio();
  buf__init();
  bt__init();
  can_bus_handler__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_handler__transmit_messages_1Hz(); }

void periodic_callbacks__10Hz(uint32_t callback_count) {}
void periodic_callbacks__100Hz(uint32_t callback_count) {
  if (callback_count % 5 == 0) {
    sensor_node__trigger_Left_ultrasonic();
    sensor_node__trigger_Middle_ultrasonic();
    sensor_node__trigger_Right_ultrasonic();
  } else if (callback_count % 5 == 4) {
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(2);
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(3);
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(4);
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(5);

    can_bus_handler__transmit_messages_100Hz();
  }
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {}