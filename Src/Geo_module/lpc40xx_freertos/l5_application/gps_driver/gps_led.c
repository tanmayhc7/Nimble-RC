#include "gps_led.h"

#include "board_io.h"
#include "gpio.h"

void gps_led__init(void) {}

void gps_led__onboard_led_gps_fix_success(void) { gpio__toggle(board_io__get_led3()); }

void gps_led__onboard_led_gps_fix_fail(void) { gpio__set(board_io__get_led3()); }