#pragma once

typedef struct {
  float latitude;
  float longitude;
} gps_coordinates_t;

void gps__init(void);
void gps__run_once(void);

void gps_print_coordinate(void);
gps_coordinates_t gps__get_coordinates(void);