#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bluetooth.h"
#include "gpio.h"
#include "line_buffer.h"
#include "uart.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "clock.h" // needed for UART initialization

// Change this according to which UART you plan to use
static uart_e gps_uart = UART__3;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[256];
static line_buffer_s line;

static bt_coordinates_t parsed_coordinates;

static void bt__absorb_data(void) {
  char byte;
  while (uart__get(gps_uart, &byte, 0)) {
    line_buffer__add_byte(&line, byte);
  }
}

/**
  Global Positioning System Fix Data. Time, position and fix related data for a GPS receiver.

  $GPGGA,hhmmss.sss,llll.llll,a,yyyyy.yyyy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh

  $GPGGA = Header
  hhmmss.sss = UTC of position
  llll.llll = latitude of position
  a = N or S
  yyyyy.yyyy = Longitude of position
  a = E or W
  x = GPS Quality indicator (0=no fix, 1=GPS fix, 2=Dif. GPS fix)
  xx = number of satellites in use
  x.x = horizontal dilution of precision
  x.x = Antenna altitude above mean-sea-level
  M = units of antenna altitude, meters
  x.x = Geoidal separation
  M = units of geoidal separation, meters
  x.x = Age of Differential GPS data (seconds)
  xxxx = Differential reference station ID
  hh = Checksum and always begin with *
*/
static void bt__handle_line(void) {
  char gps_line[100];

  char latitude_string[10];
  char longitude_string[11];
  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {
    uint8_t break_point = 0;
    uint8_t sign_lat = 1;
    uint8_t sign_lng = 1;
    // puts(gps_line);
    for (uint8_t index = 0; index < strlen(gps_line); index++) {
      if (gps_line[index] == ':') {
        break_point = index;
        break;
      }
      if (gps_line[index] == '-')
        sign_lat *= -1;
      else
        latitude_string[index] = gps_line[index];
    }
    for (uint8_t index = 0; index < strlen(longitude_string); index++) {
      if (gps_line[index] == '-')
        sign_lng *= -1;
      else
        longitude_string[index] = gps_line[index + break_point + 1];
    }
    // puts(longitude_string);
    parsed_coordinates.latitude = atof(latitude_string);
    parsed_coordinates.longitude = atof(longitude_string);
    // parsed_coordinates.latitude *= sign_lat;
    // parsed_coordinates.longitude *= sign_lng;
  }

  // Print over UART0 - micro usb
}

void bt__init(void) {
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // P4.28 as TXD3
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // P4.29 as RXD3
  // gpio__construct_as_output(GPIO__PORT_1, 14);
  // gpio_s AT = {.port_number = 1, .pin_number = 14};
  // gpio__set(AT);

  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 115200);
  // RX queue should be sized such that it can buffer data in UART driver until gps__run_once() is called
  // Note: Assuming 115200bps, we can get 115200 / 11 bits = ~10.5 chars per ms; thus ~105 characters per 10 ms @ 100 Hz
  // which is perfect for NavMan 3400 (SiRF chipset sentences) GPS lines which are about ~72 characters in length
  QueueHandle_t rxq_handle = xQueueCreate(128, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(8, sizeof(char)); // We don't send anything to the GPS
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

/// Public functions:
///
void bt__run_once(void) {
  bt__absorb_data();
  bt__handle_line();
}

bt_coordinates_t bt__get_coordinates(void) {
  // TODO return parsed_coordinates
  return parsed_coordinates;
}

dbc_GPS_DESTINATION_INFO_s bt__can_update_destination_gps_coordinates(void) {
  dbc_GPS_DESTINATION_INFO_s bridge_gps_info_s;
  bridge_gps_info_s.GPS_DESTINATION_LAT = parsed_coordinates.latitude;
  bridge_gps_info_s.GPS_DESTINATION_LONG = parsed_coordinates.longitude;
  return bridge_gps_info_s;
}