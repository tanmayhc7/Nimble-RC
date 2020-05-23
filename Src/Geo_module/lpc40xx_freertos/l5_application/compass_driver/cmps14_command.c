#include <stdio.h>

#include "cmps14_command.h"
#include "uart.h"

// Byte sequence for storing calibration profile
static const uint8_t START_SEQ_1 = 0xF0;
static const uint8_t START_SEQ_2 = 0xF5;
static const uint8_t START_SEQ_3 = 0xF6;

// Byte sequence for erasing calibration profile
static const uint8_t ERASE_SEQ_1 = 0xE0;
static const uint8_t ERASE_SEQ_2 = 0xE5;
static const uint8_t ERASE_SEQ_3 = 0xE2;

// Byte sequence for default calibration
static const uint8_t DEFAULT_SEQ_1 = 0x20;
static const uint8_t DEFAULT_SEQ_2 = 0x2A;
static const uint8_t DEFAULT_SEQ_3 = 0x60;

// Byte end calibration
static const uint8_t END_CALIBRATION = 0xF8;

static const uint8_t get_16_bit_bearing = 0x13;
static const uint8_t get_8_bit_bearing = 0x12;

static uart_e uart_3 = UART__3;
static const uint32_t zero_timeout_ms = 0;

void cmps14_command__send_get_16_bit_bearing(void) {
  if (uart__put(uart_3, get_16_bit_bearing, zero_timeout_ms)) {
  }
}

void cmps14_command__get_8_bit_bearing(void) {
  if (uart__put(uart_3, get_8_bit_bearing, zero_timeout_ms)) {
  }
}

void cmps14_command__start_calibration(void) {
  uart__put(UART__3, START_SEQ_1, 0);
  uart__put(UART__3, START_SEQ_2, 0);
  uart__put(UART__3, START_SEQ_3, 0);
}

void cmps14_command__end_calibration(void) { uart__put(UART__3, END_CALIBRATION, 0); }

void cmps14_command__erase_profile(void) {
  uart__put(UART__3, ERASE_SEQ_1, 0);
  uart__put(UART__3, ERASE_SEQ_2, 0);
  uart__put(UART__3, ERASE_SEQ_3, 0);
}

void cmps14_command__factory_calibration(void) {
  uart__put(UART__3, DEFAULT_SEQ_1, 0);
  uart__put(UART__3, DEFAULT_SEQ_2, 0);
  uart__put(UART__3, DEFAULT_SEQ_3, 0);
}