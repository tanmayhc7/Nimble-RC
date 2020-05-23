#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "line_buffer.h"
#include "uart.h"

#include "FreeRTOS.h"
#include "gpio.h"
#include "queue.h"

#include "clock.h"
#include "cmps14_command.h"
#include "compass.h"

static char cmps_line_buffer_16_bit[2];
static char cmps_line_buffer_8_bit[2];
static line_buffer_s line, line2;

static uart_e gps_uart_3 = UART__3;
static compass_data_t receive_compass_data = {0};

void compass__init(void) {
  line_buffer__init(&line, cmps_line_buffer_16_bit, sizeof(cmps_line_buffer_16_bit));
  line_buffer__init(&line2, cmps_line_buffer_8_bit, sizeof(cmps_line_buffer_8_bit));
  uart__init_2_stop_bits(gps_uart_3, clock__get_peripheral_clock_hz(), 9600);
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // TX
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // RX
  QueueHandle_t rxq_handle = xQueueCreate(20, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(20, sizeof(char));
  uart__enable_queues(gps_uart_3, rxq_handle, txq_handle);
}

static void compass__absorb_data_16_bit(void) {
  char byte;
  while (uart__get(gps_uart_3, &byte, 0)) {
    line_buffer__add_byte(&line, byte);
  }
}

static void compass__handle_line_8_bit(void) {
  static char cmps_line2[2];
  char output_byte;
  float degree_8_bit;
  float conversion_factor = 1.4114;
  if (line_buffer__remove_line(&line2, cmps_line2, sizeof(cmps_line2))) {
    output_byte = cmps_line2[0];
    degree_8_bit = ((uint8_t)output_byte * conversion_factor);
    degree_8_bit = floor(degree_8_bit * 10) / 10;
    receive_compass_data.heading_degree_8_bit = degree_8_bit;
    printf("8-bit degree: %0.1f\n", (double)receive_compass_data.heading_degree_8_bit);
  }
}

static void compass__absorb_data_8_bit(void) {
  char byte;
  while (uart__get(gps_uart_3, &byte, 0)) {
    line_buffer__add_byte(&line2, byte);
  }
}

static void compass__handle_line_16_bit(void) {
  static char cmps_line[2];
  char higher_byte, lower_byte;
  if (line_buffer__remove_line(&line, cmps_line, sizeof(cmps_line))) {
    higher_byte = cmps_line[0]; // higher byte of 16bit cmps14 data is taken first.
    lower_byte = cmps_line[1];  // lower byte of 16 bit cmps14 data.
    receive_compass_data = compass__combine_upper_and_lower_byte(higher_byte, lower_byte);
    // printf("receive compass data: %0.1f\n", (double)receive_compass_data.heading_degree_16_bit);
  }
}

compass_data_t compass__combine_upper_and_lower_byte(char upper_byte, char lower_byte) {
  uint16_t heading_deg_from_cmps14;
  compass_data_t previous_cmps14_heading_deg = receive_compass_data;
  heading_deg_from_cmps14 = (((uint16_t)(upper_byte << 8)) | (lower_byte & 0x00ff));
  if (heading_deg_from_cmps14 > 0 && heading_deg_from_cmps14 <= 3599) {
    receive_compass_data.heading_degree_16_bit = (float)heading_deg_from_cmps14 / 10;
    return receive_compass_data;
  } else {
    return previous_cmps14_heading_deg;
  }
}

void compass__run_once_8_bit(void) {
  cmps14_command__get_8_bit_bearing();
  compass__absorb_data_8_bit();
  compass__handle_line_8_bit();
}

void compass__run_once_16_bit(void) {
  cmps14_command__send_get_16_bit_bearing();
  compass__absorb_data_16_bit();
  compass__handle_line_16_bit();
}

compass_data_t compass__get_data(void) { return receive_compass_data; }
