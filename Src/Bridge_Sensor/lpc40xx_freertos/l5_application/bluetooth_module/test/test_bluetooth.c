// TODO:
// test_gps.c
#include "unity.h"

// Mocks
#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mockuart.h"

#include "Mockqueue.h"

// Use the real implementation (not mocks) for:
#include "line_buffer.h"

// Include the source we wish to test
#include "bluetooth.h"
void setUp(void) {

  gpio_s txd3 = {0};
  gpio_s rxd3 = {0};

  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 28, GPIO__FUNCTION_2, txd3); // P4.28 as TXD3
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 29, GPIO__FUNCTION_2, rxd3); // P4.29 as RXD3

  clock__get_peripheral_clock_hz_IgnoreAndReturn(0);
  uart__init_Expect(UART__3, 0, 115200U);
  uart__init_IgnoreArg_peripheral_clock();

  xQueueCreate_ExpectAndReturn(128, sizeof(char), NULL);
  xQueueCreate_ExpectAndReturn(8, sizeof(char), NULL);

  uart__enable_queues_ExpectAndReturn(UART__3, NULL, NULL, NULL);
  uart__enable_queues_IgnoreArg_queue_receive();
  uart__enable_queues_IgnoreArg_queue_transmit();
  bt__init();
}

void tearDown(void) {}

void test_bluetooth_init(void) {}
