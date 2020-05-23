#include "fake_compass.h"

static float fake_compass_heading_value = 0;

float fake_compass__get_current_heading(void) {
  fake_compass_heading_value = (int)(fake_compass_heading_value + 10) % 359;
  return fake_compass_heading_value;
}