#include <stdio.h>
#include <string.h>

#include "unity.h"
#include <stdint.h>
#include <stdlib.h>

// Include the source we wish to test
#include "buffer_handler.c"

void setUp(void) { buf__init(); }

void tearDown(void) {}

void test__buffer__initialize(void) {
  TEST_ASSERT_EQUAL_UINT8(0, buf_ptr->iterator_left);
  TEST_ASSERT_EQUAL_UINT8(0, buf_ptr->iterator_mid);
  TEST_ASSERT_EQUAL_UINT8(0, buf_ptr->iterator_right);
  TEST_ASSERT_EQUAL_UINT8(0, buf_ptr->iterator_rear);
}

void test__buffer_get_values_Left(void) {
  for (int i = 0; i < 10; i++) {
    buf__add_sensor_value_Left(i);
  }
  TEST_ASSERT_EQUAL_UINT16(5, buf_ptr->min_left);
  for (int i = 0; i < 4; i++) {
    buf__add_sensor_value_Left(i);
  }
  TEST_ASSERT_EQUAL_UINT16(0, buf_ptr->min_left);
}

void test__buffer_get_values_Middle(void) {
  for (int i = 0; i < 10; i++) {
    buf__add_sensor_value_Mid(i);
  }
  TEST_ASSERT_EQUAL_UINT16(5, buf_ptr->min_mid);
  for (int i = 0; i < 4; i++) {
    buf__add_sensor_value_Mid(i);
  }
  TEST_ASSERT_EQUAL_UINT16(0, buf_ptr->min_mid);
}

void test__buffer_get_values_Right(void) {
  for (int i = 0; i < 10; i++) {
    buf__add_sensor_value_Right(i);
  }
  TEST_ASSERT_EQUAL_UINT16(5, buf_ptr->min_right);
  for (int i = 0; i < 4; i++) {
    buf__add_sensor_value_Right(i);
  }
  TEST_ASSERT_EQUAL_UINT16(0, buf_ptr->min_right);
}

void test__buffer_get_values_Rear(void) {
  for (int i = 0; i < 10; i++) {
    buf__add_sensor_value_Rear(i);
  }
  TEST_ASSERT_EQUAL_UINT16(5, buf_ptr->min_rear);
  for (int i = 0; i < 4; i++) {
    buf__add_sensor_value_Rear(i);
  }
  TEST_ASSERT_EQUAL_UINT16(0, buf_ptr->min_rear);
}