#include <stdio.h>
#include <string.h>

#include "gps.h"

#include "gps_led.h"
#include "line_buffer.h"
#include "uart.h"

#include "FreeRTOS.h"
#include "gpio.h"
#include "queue.h"

#include "clock.h"

static uart_e gps_uart = UART__2;

static char line_buffer[256];
static line_buffer_s line;

static gps_coordinates_t parsed_coordinates = {0};

static void gps__absorb_data(void) {
  char byte;
  while (uart__get(gps_uart, &byte, 0)) {
    line_buffer__add_byte(&line, byte);
  }
}

static void gps__handle_line(void) {
  static char gps_line[200];
  static float latitude;
  static float longitude;
  static char lat_direction;
  static char long_direction;
  uint8_t valid_gps_fix;
  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {
    if (*gps_line == '$') {
      if (NULL != strstr(gps_line, "$GPGGA")) {
        sscanf(gps_line, "$GPGGA, %*f, %f, %c, %f, %c,%hhi,%*i,%*s", &latitude, &lat_direction, &longitude,
               &long_direction, &valid_gps_fix);
        if (valid_gps_fix > 0) {
          if (long_direction == 'W' || long_direction == 'w') {
/*
  if (1) Using SJSU CAMPUS gps calibrations settings
     (0) Using Plan B (Gordy Street Calibration Settings)
*/
#if (0)
            parsed_coordinates.longitude =
                (longitude + 35.2741) * -1; // Longitude Calibration for SJSU CAMPUS(7th Street)
#else
            parsed_coordinates.longitude =
                (longitude + 35.2624) * -1; // Longitude Calibration for Plan B (Gordy Street)
#endif
          } else {
            parsed_coordinates.longitude = longitude;
          }
          if (lat_direction == 'S' || long_direction == 's') {
            parsed_coordinates.latitude = latitude * -1;
          } else {
            parsed_coordinates.latitude = latitude;
#if (0)
// parsed_coordinates.latitude = (parsed_coordinates.latitude + 3.4599); // Latitude Calibration for SJSU CAMPUS(7th
// Street)
#else
            parsed_coordinates.latitude =
                (parsed_coordinates.latitude + 15.4220); // Latitude Calibration for Plan B (Gordy Street)
#endif
          }
          // Toggle LED indicator
          gps_led__onboard_led_gps_fix_success();
          parsed_coordinates.latitude = parsed_coordinates.latitude / 100;
          parsed_coordinates.longitude = parsed_coordinates.longitude / 100;
        } else {
          // do nothing
          // do not parse invalid gps fix
          // Toggle LED indicator
          gps_led__onboard_led_gps_fix_fail();
        }
        // printf("gps line: %s\n", gps_line);
        // printf("latitude: %f\n", (double)parsed_coordinates.latitude);
        // printf("longitude: %f\n", (double)parsed_coordinates.longitude);
      }
    }
  }
}

void gps__init(void) {
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 9600);
  gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_2, 9, GPIO__FUNCTION_2);
  QueueHandle_t rxq_handle = xQueueCreate(200, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(8, sizeof(char));
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

void gps__run_once(void) {
  gps__absorb_data();
  gps__handle_line();
}

gps_coordinates_t gps__get_coordinates(void) { return parsed_coordinates; }