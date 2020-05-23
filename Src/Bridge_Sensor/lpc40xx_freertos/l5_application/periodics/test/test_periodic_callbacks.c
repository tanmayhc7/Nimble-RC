#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockbluetooth.h"
#include "Mockboard_io.h"
#include "Mockbuffer_handler.h"
#include "Mockcan_handler.h"
#include "Mockgpio.h"
#include "Mocksensor_get_value.h"
#include "Mocksensor_initializer.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  sensor_node__init_all_sensors_Expect();
  sensor_node__init_trigger_pins_as_gpio_Expect();
  buf__init_Expect();
  bt__init_Expect();
  can_bus_handler__init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  can_handler__transmit_messages_1Hz_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__100Hz(void) {
  for (int i = 0; i < 10; i += 5) {
    sensor_node__trigger_Left_ultrasonic_Expect();
    sensor_node__trigger_Middle_ultrasonic_Expect();
    sensor_node__trigger_Right_ultrasonic_Expect();
  }
  uint16_t val;
  for (int i = 4; i < 10; i += 5) {
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz_Expect(2);
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz_Expect(3);
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz_Expect(4);
    sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz_Expect(5);
    can_bus_handler__transmit_messages_100Hz_Expect();
  }
  periodic_callbacks__100Hz(0);
  periodic_callbacks__100Hz(4);
  periodic_callbacks__100Hz(5);
  periodic_callbacks__100Hz(9);
}
