#include <math.h>
#include <stdio.h>

#include "adc.h"
#include "buffer_handler.h"
#include "delay.h"
#include "gpio.h"
#include "sensor_get_value.h"

static const uint8_t trigger_delay_time = 30;
/***************************************
 *      Trigger Pins & Functions
 **************************************/
static gpio_s trigger_left, trigger_mid, trigger_right;
void sensor_node__init_trigger_pins_as_gpio(void) {
  trigger_left = gpio__construct_as_output(0, 6);
  trigger_mid = gpio__construct_as_output(0, 7);
  trigger_right = gpio__construct_as_output(0, 8);
}

void sensor_node__trigger_Left_ultrasonic(void) {
  gpio__set(trigger_left);
  delay__us(trigger_delay_time);
  gpio__reset(trigger_left);
}

void sensor_node__trigger_Middle_ultrasonic(void) {
  gpio__set(trigger_mid);
  delay__us(trigger_delay_time);
  gpio__reset(trigger_mid);
}

void sensor_node__trigger_Right_ultrasonic(void) {
  gpio__set(trigger_right);
  delay__us(trigger_delay_time);
  gpio__reset(trigger_right);
}

/*******************************
 *    Sensor Read Functions
 *******************************/
void sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(sensor_e sensor_pos) {
  uint16_t val = 0;
  char buffer_term[100];
  switch (sensor_pos) {
  case 2:
    val = 0.3182 * adc__get_adc_value(2) - 0.3959;
    sprintf(buffer_term, "Left: %d \n", val);
    puts(buffer_term);
    buf__add_sensor_value_Left(val);
    break;
  case 3:
    val = 0.3182 * adc__get_adc_value(3) - 0.3959;
    sprintf(buffer_term, "Mid: %d \n", val);
    puts(buffer_term);
    buf__add_sensor_value_Mid(val);
    break;
  case 4:
    val = 0.3182 * adc__get_adc_value(4) - 0.3959;
    sprintf(buffer_term, "Right: %d \n", val);
    puts(buffer_term);
    buf__add_sensor_value_Right(val);
    break;
  case 5:
    val = 227648 * pow(adc__get_adc_value(5), -1.27);
    sprintf(buffer_term, "Rear: %d \n", val);
    puts(buffer_term);
    buf__add_sensor_value_Rear(val);
    break;
  default:
    puts("default");
    break;
  }
}