#include <stdint.h>

#include "cmps14_command.h"
#include "cmps_button_app.h"

#include "board_io.h"
#include "gpio.h"

void cmps_button_app__periodic_1Hz(void) {
  if (gpio__get(board_io__get_sw0())) {
    cmps14_command__end_calibration();
  } else if (gpio__get(board_io__get_sw1())) {
    cmps14_command__start_calibration();
  } else if (gpio__get(board_io__get_sw2())) {
    cmps14_command__factory_calibration();
  } else if (gpio__get(board_io__get_sw3())) {
    cmps14_command__erase_profile();
  }
}
