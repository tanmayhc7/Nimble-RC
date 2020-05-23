#include "unity.h"

#include "Mockboard_io.h"
#include "Mockgpio.h"

#include "gps_led.h"

void test_gps_led__onboard_led_gps_fix_success(void) {
  gpio_s gpio = {};
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  gps_led__onboard_led_gps_fix_success();
}

void test_gps_led__onboard_led_gps_fix_fail(void) {
  gpio_s gpio = {};
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  gps_led__onboard_led_gps_fix_fail();
}