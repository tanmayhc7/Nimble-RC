#include "unity.h"

// Include the source we wish to test
#include "line_buffer.h"

static line_buffer_s line_buffer;
static char memory[8];

// This method re-initializes the line_buffer for the rest of the tests
void setUp(void) { line_buffer__init(&line_buffer, memory, sizeof(memory)); }

void tearDown(void) {}

void test_line_buffer_init(void) {
  TEST_ASSERT_EQUAL(line_buffer.write_index, 0);
  TEST_ASSERT_EQUAL(line_buffer.read_index, 0);
  TEST_ASSERT_EQUAL(line_buffer.max_size, 8);
}

void test_line_buffer__nominal_case(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, 'c');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abc");
}

void test_line_buffer__return_carriage_newline(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, 'c');
  line_buffer__add_byte(&line_buffer, '\r');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abc");
}

void test_line_buffer__multiple_lines(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, '\n');
  line_buffer__add_byte(&line_buffer, 'c');
  line_buffer__add_byte(&line_buffer, 'd');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "ab");

  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "cd");
}

void test_line_buffer__overflow_case(void) {
  // Add chars until full capacity
  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }

  // Buffer should be full now
  TEST_ASSERT_FALSE(line_buffer__add_byte(&line_buffer, 'b'));

  // Retreive truncated output (without the newline char)
  // Do not modify this test; instead, change your API to make this test pass
  char line[8] = {0};
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abcdefg");
}

void test_line_buffer__multiple_with_new_carriage(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, '\r');
  line_buffer__add_byte(&line_buffer, '\n');
  line_buffer__add_byte(&line_buffer, 'g');
  line_buffer__add_byte(&line_buffer, 'g');
  line_buffer__add_byte(&line_buffer, '\r');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "ab");
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "gg");
}

void test_line_buffer__overflow_with_new_data(void) {
  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }
  TEST_ASSERT_FALSE(line_buffer__add_byte(&line_buffer, 'b'));

  char line[8] = {0};
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abcdefg");

  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'e'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'l'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'l'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'o'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "hello");
}

static line_buffer_s fake_gps_line_buffer;
static char fake_gps_memory[200];

void test_line_buffer__add_remove_line(void) {
  line_buffer__init(&fake_gps_line_buffer, fake_gps_memory, sizeof(fake_gps_memory));
  static char test_string[] = "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n$GPGGA,123519,4807.038,N,01131.000,E,"
                              "1,08,0.9,545.4,M,46.9,M,,*47\n$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n";
  line_buffer__init(&fake_gps_line_buffer, fake_gps_memory, sizeof(fake_gps_memory));
  for (size_t i = 0; i < sizeof(test_string); i++) {
    line_buffer__add_byte(&fake_gps_line_buffer, test_string[i]);
  }
  TEST_ASSERT_EQUAL_STRING(fake_gps_memory,
                           "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n$GPGGA,123519,4807.038,N,01131.000,E,"
                           "1,08,0.9,545.4,M,46.9,M,,*47\n$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n");
  char data[100];
  line_buffer__remove_line(&fake_gps_line_buffer, data, sizeof(data));
  TEST_ASSERT_EQUAL_STRING(data, "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39");
  line_buffer__remove_line(&fake_gps_line_buffer, data, sizeof(data));
  TEST_ASSERT_EQUAL_STRING(data, "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
  line_buffer__remove_line(&fake_gps_line_buffer, data, sizeof(data));
  TEST_ASSERT_EQUAL_STRING(data, "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39");
}
