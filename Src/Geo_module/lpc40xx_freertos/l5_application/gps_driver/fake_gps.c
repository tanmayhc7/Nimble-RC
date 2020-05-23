#include "fake_gps.h"

#include "gpio.h"
#include "stdio.h"
#include "uart.h"
#include "uart_printf.h"

#include "clock.h" // needed for UART initialization

static uart_e gps_uart = UART__2;

void fake_gps__init(void) {
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 115200);
  gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCTION_2); // TX
  gpio__construct_with_function(GPIO__PORT_2, 9, GPIO__FUNCTION_2); // RX
  QueueHandle_t rxq_handle = xQueueCreate(4, sizeof(char));         // Nothing to receive
  QueueHandle_t txq_handle = xQueueCreate(100, sizeof(char));       // We send a lot of data
  uart__enable_queues(gps_uart, txq_handle, rxq_handle);
}

/// Send a fake GPS string
/// TODO: You may want to be somewhat random about the coordinates that you send here
void fake_gps__run_once(void) {
  static float longitude;
  uart_printf(gps_uart, "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n");
  longitude += 1.15; // random incrementing value
}