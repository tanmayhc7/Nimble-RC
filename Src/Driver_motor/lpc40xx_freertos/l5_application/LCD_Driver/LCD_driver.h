#pragma once
#include <stdint.h>

void LCD__init(void);

void LCD__reset(void);

void LCD__char_to_LCD(uint8_t character);

void LCD__string_to_LCD(char *input_string);

void LCD__command_to_LCD(uint8_t command);