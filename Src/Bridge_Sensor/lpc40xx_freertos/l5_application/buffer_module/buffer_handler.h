#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "sensor_get_value.h"

typedef struct {
  uint16_t buf_left[5];
  uint16_t buf_right[5];
  uint16_t buf_mid[5];
  uint16_t buf_rear[5];
  unsigned int iterator_left : 3;
  unsigned int iterator_right : 3;
  unsigned int iterator_mid : 3;
  unsigned int iterator_rear : 3;
  uint16_t min_left;
  uint16_t min_right;
  uint16_t min_mid;
  uint16_t min_rear;
} buffer_S;

void buf__init(void);

void buf__add_sensor_value_Left(uint16_t value);
void buf__add_sensor_value_Right(uint16_t value);
void buf__add_sensor_value_Mid(uint16_t value);
void buf__add_sensor_value_Rear(uint16_t value);
uint16_t buf__return_min_value(sensor_e sensor_pos);