#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockadc.h"

// Include the source we wish to test
#include "sensor_initializer.h"

void setUp(void) {}

void tearDown(void) {}

void test__sensor_init(void) {
  adc__initialize_Expect();
  adc__setup_iocon_registers_Expect(2); // Left
  adc__setup_iocon_registers_Expect(3); // Middle
  adc__setup_iocon_registers_Expect(4); // Right
  adc__setup_iocon_registers_Expect(5); // Rear
  sensor_node__init_all_sensors();
}