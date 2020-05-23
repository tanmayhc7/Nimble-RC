#pragma once
typedef enum { left = 2, middle, right, rear } sensor_e;
void sensor_node__read_sensor_in_cms_value_n_add_to_buffer_100Hz(sensor_e sensor_type);
void sensor_node__trigger_Left_ultrasonic(void);
void sensor_node__trigger_Middle_ultrasonic(void);
void sensor_node__trigger_Right_ultrasonic(void);
void sensor_node__init_trigger_pins_as_gpio(void);