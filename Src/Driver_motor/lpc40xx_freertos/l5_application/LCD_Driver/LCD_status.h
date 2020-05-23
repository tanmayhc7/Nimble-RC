#pragma once
#include "project.h"

void LCD_status__update_sensor(dbc_SENSOR_DATA_s *sensor_data);
void LCD_status__update_compass(dbc_COMPASS_s *compass_data);
void LCD_status__update_motor_debug(dbc_MOTOR_DATA_s *motor_data_s);

void LCD_status__display_screen(void);
void LCD_status__display_handle_1hz(void);
