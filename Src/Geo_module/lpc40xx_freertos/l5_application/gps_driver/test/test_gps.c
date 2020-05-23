#include "unity.h"

#include "Mockclock.h"
#include "Mockuart.h"

#include "Mockgpio.h"
#include "Mockgps_led.h"
#include "Mockqueue.h"

#include "line_buffer.h"

#include "gps.c"

static char line_buffer[256];
static line_buffer_s line;
static gps_coordinates_t test_parsed_coordinates = {0};

void setUp(void) { line_buffer__init(&line, line_buffer, sizeof(line_buffer)); }

void tearDown(void) {}

void test_gps_init(void) {
  QueueHandle_t rxq = {0};
  QueueHandle_t txq = {0};
  gpio_s gpio;
  uint32_t clock_peripheral;
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  TEST_ASSERT_EQUAL(256, line.max_size);

  uart__init_Expect(gps_uart, clock_peripheral, 9600);
  clock__get_peripheral_clock_hz_IgnoreAndReturn(clock_peripheral);
  uart__enable_queues_ExpectAndReturn(gps_uart, rxq, txq, true);

  xQueueCreate_ExpectAndReturn(200, sizeof(char), rxq);
  xQueueCreate_ExpectAndReturn(8, sizeof(char), txq);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_2, 8, GPIO__FUNCTION_2, gpio);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_2, 9, GPIO__FUNCTION_2, gpio);
  gps__init();
}

bool uart__get_stub_NW(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char fake_gps_string[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n";
  if (i < sizeof(fake_gps_string)) {
    *input_byte = fake_gps_string[i++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_stub_NE(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char fake_gps_string[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,E,1,8,1.03,61.7,M,55.2,M,,*76\r\n";
  if (i < sizeof(fake_gps_string)) {
    *input_byte = fake_gps_string[i++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_stub_SW(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char fake_gps_string[] = "$GPGGA,092750.000,5321.6802,S,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n";
  if (i < sizeof(fake_gps_string)) {
    *input_byte = fake_gps_string[i++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_stub_SE(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char fake_gps_string[] = "$GPGGA,092750.000,5321.6802,S,00630.3372,E,1,8,1.03,61.7,M,55.2,M,,*76\r\n";
  if (i < sizeof(fake_gps_string)) {
    *input_byte = fake_gps_string[i++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_stub_multiple_valid_sentences(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char fake_gps_string[] =
      "$GPGGA,092750.000,5351.6802,S,00730.3372,E,2,\r\n$GPGGA,092750.000,5321.6802,S,00630."
      "3372,E,1,8,1.03,61.7,M,55.2,M,,*76\r\n";
  if (i < sizeof(fake_gps_string)) {
    *input_byte = fake_gps_string[i++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_stub_invalid_gps_fix(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int i = 0;
  static char fake_gps_string[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,0,8,1.03,61.7,M,55.2,M,,*76\r\n";
  if (i < sizeof(fake_gps_string)) {
    *input_byte = fake_gps_string[i++];
    return true;
  } else {
    return false;
  }
}

void test_gps_absorb_data_to_line_memory(void) {
  char byte;
  uart__get_StubWithCallback(uart__get_stub_NW);
  gps__absorb_data();
  TEST_ASSERT_EQUAL_STRING(((char *)line.memory),
                           "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n");
}
/*
          if(1) Using SJSU CAMPUS gps calibrations settings
            (0) Using Plan B (Gordy Street calibration settings)
        */
#if (0)
void test_gps_absorb_and_handle_line_and_get_gps_coordinates_degrees_NW(void) {
  char gps_line[100];

  char byte;
  uart__get_StubWithCallback(uart__get_stub_NW);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(53.216802 + .134599, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-6.303372 - .352741, test_parsed_coordinates.longitude);
}

void test_gps_absorb_and_handle_line_and_get_gps_coordinates_degrees_NE(void) {
  char gps_line[100];

  char byte;
  uart__get_StubWithCallback(uart__get_stub_NE);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(53.216802 + .134599, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(6.303372, test_parsed_coordinates.longitude);
}

void test_gps_absorb_and_handle_line_and_get_gps_coordinates_degrees_SW(void) {
  char gps_line[100];

  char byte;
  uart__get_StubWithCallback(uart__get_stub_SW);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(-53.216802, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-6.303372 - .352741, test_parsed_coordinates.longitude);
}

#else
void test_gps_absorb_and_handle_line_and_get_gps_coordinates_degrees_NW(void) {
  char gps_line[100];

  char byte;
  uart__get_StubWithCallback(uart__get_stub_NW);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(53.216802 + .154220, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-6.303372 - .352624, test_parsed_coordinates.longitude);
}

void test_gps_absorb_and_handle_line_and_get_gps_coordinates_degrees_NE(void) {
  char gps_line[100];

  char byte;
  uart__get_StubWithCallback(uart__get_stub_NE);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(53.216802 + .154220, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(6.303372, test_parsed_coordinates.longitude);
}

void test_gps_absorb_and_handle_line_and_get_gps_coordinates_degrees_SW(void) {
  char gps_line[100];

  char byte;
  uart__get_StubWithCallback(uart__get_stub_SW);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(-53.216802, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-6.303372 - .352624, test_parsed_coordinates.longitude);
}
#endif

void test_gps_absorb_and_handle_line_and_get_gps_coordinates_degrees_SE(void) {
  char gps_line[100];

  char byte;
  uart__get_StubWithCallback(uart__get_stub_SE);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(-53.216802, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(6.303372, test_parsed_coordinates.longitude);
}

void test_gps_handle_invalid_gps_fix_2(void) {
  char gps_line[200];
  char byte;
  uart__get_StubWithCallback(uart__get_stub_multiple_valid_sentences);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(-53.5168, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(7.30337, test_parsed_coordinates.longitude);

  gps_led__onboard_led_gps_fix_success_Expect();
  gps__handle_line();
  test_parsed_coordinates = gps__get_coordinates();

  TEST_ASSERT_EQUAL_FLOAT(-53.2168, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(6.30337, test_parsed_coordinates.longitude);
}

void test_gps_handle_invalid_gps_fix(void) {
  char gps_line[100];
  test_parsed_coordinates.latitude = 0;
  test_parsed_coordinates.longitude = 0;
  char byte;
  uart__get_StubWithCallback(uart__get_stub_invalid_gps_fix);
  gps__absorb_data();

  gps_led__onboard_led_gps_fix_fail_Expect();
  gps__handle_line();

  TEST_ASSERT_EQUAL_FLOAT(0, test_parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(0, test_parsed_coordinates.longitude);
}
