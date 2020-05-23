#include <stdio.h>

#include "LCD_driver.h"
#include "LCD_status.h"
#include "string.h"

static dbc_SENSOR_DATA_s lcd_sensor = {0};
static dbc_COMPASS_s lcd_compass = {0};
static dbc_MOTOR_DATA_s lcd_motor = {0};

void LCD_status__update_sensor(dbc_SENSOR_DATA_s *sensor_data) { lcd_sensor = *sensor_data; }

void LCD_status__update_compass(dbc_COMPASS_s *compass_data) { lcd_compass = *compass_data; }

void LCD_status__update_motor_debug(dbc_MOTOR_DATA_s *motor_data_s) { lcd_motor = *motor_data_s; }

void LCD_status__display_on_screen(void) {
  char lcd_buffer[20];
  LCD__command_to_LCD(0x01);
  // Clear Screen before update
  LCD__command_to_LCD(0x80);
  sprintf(lcd_buffer, "M: %i", lcd_sensor.SENSOR_SONARS_mid);
  LCD__string_to_LCD(lcd_buffer);

  LCD__command_to_LCD(0x8A);
  sprintf(lcd_buffer, "L: %i", lcd_sensor.SENSOR_SONARS_left);
  LCD__string_to_LCD(lcd_buffer);

  LCD__command_to_LCD(0xC0);
  sprintf(lcd_buffer, "Rr: %i", lcd_sensor.SENSOR_IR_rear);
  LCD__string_to_LCD(lcd_buffer);

  LCD__command_to_LCD(0xCA);
  sprintf(lcd_buffer, "R: %i", lcd_sensor.SENSOR_SONARS_right);
  LCD__string_to_LCD(lcd_buffer);

  LCD__command_to_LCD(0x94);
  sprintf(lcd_buffer, "CMP: %0.1f", (double)lcd_compass.CMP_CURRENT_HEADING);
  LCD__string_to_LCD(lcd_buffer);

  LCD__command_to_LCD(0x9F);
  sprintf(lcd_buffer, "m: %0.1f", (double)lcd_compass.CMP_DISTANCE);
  LCD__string_to_LCD(lcd_buffer);

  LCD__command_to_LCD(0xD4);
  sprintf(lcd_buffer, "BRG: %0.1f", lcd_compass.CMP_DEST_BEARING);
  LCD__string_to_LCD(lcd_buffer);

  LCD__command_to_LCD(0xDF);
  sprintf(lcd_buffer, "RPM: %li", lcd_motor.MOTOR_DATA_RPM);
  LCD__string_to_LCD(lcd_buffer);
}

void LCD_status__display_handle_1hz(void) { LCD_status__display_on_screen(); }