#pragma once

typedef struct {
  float heading_degree_16_bit;
  float heading_degree_8_bit;
} compass_data_t;

void compass__init(void);
void compass__run_once_16_bit(void);
void compass__run_once_8_bit(void);

compass_data_t compass__get_data(void);

compass_data_t compass__combine_upper_and_lower_byte(char upper_byte, char lower_byte);
