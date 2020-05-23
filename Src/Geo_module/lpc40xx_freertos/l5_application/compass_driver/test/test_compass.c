#include "unity.h"
#include <stdint.h>

#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mockqueue.h"
#include "Mockuart.h"

#include "cmps14_command.h"
#include "compass.c"
#include "line_buffer.h"

static char line_buffer[2];
static line_buffer_s line;
static compass_data_t test_receive_compass_data = {0};

void setUp(void) { line_buffer__init(&line, line_buffer, sizeof(line_buffer)); }
void tearDown(void) {}

void test_compass__init(void) {
  QueueHandle_t rxq = {0};
  QueueHandle_t txq = {0};
  gpio_s gpio;
  uint32_t clock_peripheral;
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  TEST_ASSERT_EQUAL(2, line.max_size);
  uart__init_2_stop_bits_Expect(gps_uart_3, clock_peripheral, 9600);
  clock__get_peripheral_clock_hz_IgnoreAndReturn(clock_peripheral);
  uart__enable_queues_ExpectAndReturn(gps_uart_3, rxq, txq, true);

  xQueueCreate_ExpectAndReturn(20, sizeof(char), rxq);
  xQueueCreate_ExpectAndReturn(20, sizeof(char), txq);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 28, GPIO__FUNCTION_2, gpio);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 29, GPIO__FUNCTION_2, gpio);
  compass__init();
}

bool uart__get_stub_eof(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  char compass_16_bit_heading[2];
  compass_16_bit_heading[0] = 0xE;
  compass_16_bit_heading[1] = 0xF;
  if (i < sizeof(compass_16_bit_heading)) {
    *input_byte = compass_16_bit_heading[i++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_stub_invalid_data(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char compass_16_bit_heading[2];
  compass_16_bit_heading[0] = 0xF;
  compass_16_bit_heading[1] = 0x10;
  if (i < sizeof(compass_16_bit_heading)) {
    *input_byte = compass_16_bit_heading[i++];
    return true;
  } else {
    return false;
  }
}

void test_compass_combine_upper_and_lower_byte(void) {
  char upper_byte = 0x7;
  char lower_byte = 0x0D;
  test_receive_compass_data = compass__combine_upper_and_lower_byte(upper_byte, lower_byte);
  TEST_ASSERT_EQUAL_FLOAT(180.5, test_receive_compass_data.heading_degree_16_bit);
}

void test_compass__absorb_data(void) {
  char byte;
  uart__get_StubWithCallback(uart__get_stub_eof);
  compass__absorb_data_16_bit();
  TEST_ASSERT_EQUAL_STRING(((char *)line.memory), "\x0E\x0F");
}

void test_compass_absorb_and_handle_line_and_get_compass_heading_16_bit(void) {
  char cmp_line[2];
  uart__put_ExpectAndReturn(UART__3, 0x13, 0, true);
  uart__get_StubWithCallback(uart__get_stub_eof);
  compass__run_once_16_bit();
  receive_compass_data = compass__get_data();
  TEST_ASSERT_EQUAL_FLOAT(359.9, receive_compass_data.heading_degree_16_bit);
}

void test_compass_handle_invalid_data(void) {
  char cmp_line[2];
  compass_data_t test_compass_data;
  test_compass_data.heading_degree_16_bit = 0;
  uart__put_ExpectAndReturn(UART__3, 0x13, 0, true);
  uart__get_StubWithCallback(uart__get_stub_invalid_data);
  compass__run_once_16_bit();
  receive_compass_data = compass__get_data();

  TEST_ASSERT_EQUAL_FLOAT(0, test_compass_data.heading_degree_16_bit);
}

bool uart__get_stub_8_bit_degree_hex_FF(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char compass_8_bit_heading[2];
  compass_8_bit_heading[0] = 0xFF;
  if (i < sizeof(compass_8_bit_heading)) {
    *input_byte = compass_8_bit_heading[i++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_stub_8_bit_degree_hex_zero(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char compass_8_bit_heading[2];
  compass_8_bit_heading[0] = 0;
  if (i < sizeof(compass_8_bit_heading)) {
    *input_byte = compass_8_bit_heading[i++];
    return true;
  } else {
    return false;
  }
}

void test_absorb_and_handle_data_for_8_bit_bearing_degree_255(void) {
  uint8_t timeout_ms = 0;
  uart__put_ExpectAndReturn(UART__3, 0x12, timeout_ms, true);
  uart__get_StubWithCallback(uart__get_stub_8_bit_degree_hex_FF);
  compass__run_once_8_bit();
  test_receive_compass_data = compass__get_data();
  TEST_ASSERT_EQUAL_FLOAT(test_receive_compass_data.heading_degree_8_bit, 359.9);
}

void test_absorb_and_handle_data_for_8_bit_bearing_degree_zero(void) {
  uint8_t timeout_ms = 0;
  uart__put_ExpectAndReturn(UART__3, 0x12, timeout_ms, true);
  uart__get_StubWithCallback(uart__get_stub_8_bit_degree_hex_zero);
  compass__run_once_8_bit();
  test_receive_compass_data = compass__get_data();
  TEST_ASSERT_EQUAL_FLOAT(test_receive_compass_data.heading_degree_8_bit, 0);
}