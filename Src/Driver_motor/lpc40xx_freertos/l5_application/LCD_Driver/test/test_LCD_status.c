#include "unity.h"
#include <stdio.h>

#include "MockLCD_driver.h"

#include "LCD_status.c"

void test_LCD_status__update_sensor(void) {
  dbc_SENSOR_DATA_s incoming_sensor_data = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = 10,
      .SENSOR_SONARS_right = 20,
      .SENSOR_SONARS_mid = 30,
      .SENSOR_IR_rear = 40,
  };
  LCD_status__update_sensor(&incoming_sensor_data);
  TEST_ASSERT_EQUAL(lcd_sensor.SENSOR_SONARS_left, 10);
  TEST_ASSERT_EQUAL(lcd_sensor.SENSOR_SONARS_right, 20);
  TEST_ASSERT_EQUAL(lcd_sensor.SENSOR_SONARS_mid, 30);
  TEST_ASSERT_EQUAL(lcd_sensor.SENSOR_IR_rear, 40);
}

void test_LCD_status__update_compass(void) {
  dbc_COMPASS_s incoming_compass_data = {
      .mia_info.mia_counter = 0,
      .CMP_DEST_BEARING = 200,
      .CMP_CURRENT_HEADING = 180,
  };
  LCD_status__update_compass(&incoming_compass_data);
  TEST_ASSERT_EQUAL(lcd_compass.CMP_DEST_BEARING, 200);
  TEST_ASSERT_EQUAL(lcd_compass.CMP_CURRENT_HEADING, 180);
}

void test_LCD_status__display_on_screen(void) {
  char M_test_buf[20];
  char L_test_buf[20];
  char R_test_buf[20];
  char Rr_test_buf[20];
  char CMPS_test_buf[20];
  char DST_test_buf[20];
  char BRG_test_buf[20];
  char RPM_test_buf[20];
  dbc_SENSOR_DATA_s incoming_sensor_data = {
      .mia_info.mia_counter = 0,
      .SENSOR_SONARS_left = 10,
      .SENSOR_SONARS_right = 20,
      .SENSOR_SONARS_mid = 30,
      .SENSOR_IR_rear = 40,
  };
  LCD_status__update_sensor(&incoming_sensor_data);

  dbc_COMPASS_s incoming_compass_data = {
      .mia_info.mia_counter = 0,
      .CMP_DEST_BEARING = 200,
      .CMP_CURRENT_HEADING = 180,
  };

  LCD__command_to_LCD_Expect(0x01);
  // Clear Screen before update
  LCD__command_to_LCD_Expect(0x80);
  sprintf(M_test_buf, "M: %i", lcd_sensor.SENSOR_SONARS_mid);
  LCD__string_to_LCD_Expect(M_test_buf);

  LCD__command_to_LCD_Expect(0x8A);
  sprintf(L_test_buf, "L: %i", lcd_sensor.SENSOR_SONARS_left);
  LCD__string_to_LCD_Expect(L_test_buf);

  LCD__command_to_LCD_Expect(0xC0);
  sprintf(Rr_test_buf, "Rr: %i", lcd_sensor.SENSOR_IR_rear);
  LCD__string_to_LCD_Expect(Rr_test_buf);

  LCD__command_to_LCD_Expect(0xCA);
  sprintf(R_test_buf, "R: %i", lcd_sensor.SENSOR_SONARS_right);
  LCD__string_to_LCD_Expect(R_test_buf);

  LCD__command_to_LCD_Expect(0x94);
  sprintf(CMPS_test_buf, "CMP: %0.1f", (double)lcd_compass.CMP_CURRENT_HEADING);
  LCD__string_to_LCD_Expect(CMPS_test_buf);

  LCD__command_to_LCD_Expect(0x9F);
  sprintf(DST_test_buf, "m: %0.1f", (double)lcd_compass.CMP_DISTANCE);
  LCD__string_to_LCD_Expect(DST_test_buf);

  LCD__command_to_LCD_Expect(0xD4);
  sprintf(BRG_test_buf, "BRG: %0.1f", lcd_compass.CMP_DEST_BEARING);
  LCD__string_to_LCD_Expect(BRG_test_buf);

  LCD__command_to_LCD_Expect(0xDF);
  sprintf(RPM_test_buf, "RPM: %li", lcd_motor.MOTOR_DATA_RPM);
  LCD__string_to_LCD_Expect(RPM_test_buf);

  LCD_status__display_on_screen();
}