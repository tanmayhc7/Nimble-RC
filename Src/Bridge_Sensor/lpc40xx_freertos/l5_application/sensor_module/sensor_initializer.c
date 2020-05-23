#include "sensor_initializer.h"
#include "adc.h"

void sensor_node__init_all_sensors(void) {
  adc__initialize();
  adc__setup_iocon_registers(2); // Left Ultrasonic
  adc__setup_iocon_registers(3); // Middle Ultrasonic
  adc__setup_iocon_registers(4); // Right Ultrasonic
  adc__setup_iocon_registers(5); // Rear
}