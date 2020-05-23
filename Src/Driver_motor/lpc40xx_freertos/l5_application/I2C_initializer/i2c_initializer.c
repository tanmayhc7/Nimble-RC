#include <stdio.h>

#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "i2c_initializer.h"

static const uint8_t lcd_slave_addr = 0x4E;

void i2c_initializer__lcd_init(void) {
  gpio__construct_with_function(GPIO__PORT_0, 10, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_0, 11, GPIO__FUNCTION_2);
  i2c__initialize(I2C__2, 100000U, clock__get_peripheral_clock_hz());

  if (i2c__detect(I2C__2, lcd_slave_addr)) {
    printf("LCD Slave Addr(0x4E) detected!\n");
  }
}