#include <math.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockadc.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
//#include "Mocksensor_initializer.h"

#include "buffer_handler.h"
#include "sensor_get_value.h"

static gpio_s trigger_left, trigger_mid, trigger_right;
static const uint8_t trigger_delay_time = 30;

void setUp(void) { buf__init(); }

void tearDown(void) {}

void test__sensor_setup_triggers(void) {
  gpio__construct_as_output_ExpectAndReturn(0, 6, trigger_left);
  gpio__construct_as_output_ExpectAndReturn(0, 7, trigger_mid);
  gpio__construct_as_output_ExpectAndReturn(0, 8, trigger_right);
  sensor_node__init_trigger_pins_as_gpio();
}

void test__sensor_values(void) {
  gpio__set_Expect(trigger_left);
  delay__us_Expect(trigger_delay_time);
  gpio__reset_Expect(trigger_left);
  sensor_node__trigger_Left_ultrasonic();
  uint16_t val;
  adc__get_adc_value_ExpectAndReturn(2, val);
  buf__add_sensor_value_Left(val);
  sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(2);

  gpio__set_Expect(trigger_mid);
  delay__us_Expect(trigger_delay_time);
  gpio__reset_Expect(trigger_mid);
  sensor_node__trigger_Left_ultrasonic();
  adc__get_adc_value_ExpectAndReturn(3, val);
  buf__add_sensor_value_Mid(val);
  sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(3);

  gpio__set_Expect(trigger_right);
  delay__us_Expect(trigger_delay_time);
  gpio__reset_Expect(trigger_right);
  sensor_node__trigger_Left_ultrasonic();
  adc__get_adc_value_ExpectAndReturn(4, val);
  buf__add_sensor_value_Right(val);
  sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(4);

  adc__get_adc_value_ExpectAndReturn(5, val);
  buf__add_sensor_value_Rear(val);
  sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(5);
}