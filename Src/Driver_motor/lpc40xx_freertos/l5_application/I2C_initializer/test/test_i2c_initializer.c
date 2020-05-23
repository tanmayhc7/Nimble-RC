#include "unity.h"

#include "Mockgpio.h"
#include "Mocki2c.h"

#include "clock.h"
#include "i2c_initializer.h"

void test_i2c_initializer(void) {
  gpio_s gpio = {};
  uint32_t p_clock = clock__get_peripheral_clock_hz();
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_0, 10, GPIO__FUNCTION_2, gpio);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_0, 11, GPIO__FUNCTION_2, gpio);

  i2c__initialize_Expect(I2C__2, 100000U, p_clock);

  i2c__detect_ExpectAndReturn(I2C__2, 0x4E, true);
  i2c_initializer__lcd_init();
}