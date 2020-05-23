#include "unity.h"

#include "Mockuart.h"

#include "cmps14_command.c"

void test_cmps14_command__send_get_16_bit_bearing(void) {
  uart__put_ExpectAndReturn(UART__3, 0x13, 0, true);
  cmps14_command__send_get_16_bit_bearing();
}

void test_cmps14_command__get_8_bit_bearing(void) {
  uart__put_ExpectAndReturn(UART__3, 0x12, 0, true);
  cmps14_command__get_8_bit_bearing();
}