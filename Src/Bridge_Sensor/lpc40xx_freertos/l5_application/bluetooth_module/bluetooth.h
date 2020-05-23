#pragma once
#include "project.h"

typedef struct {
  float latitude;
  float longitude;
} bt_coordinates_t;

void bt__init(void);

void bt__run_once(void);

bt_coordinates_t bt__get_coordinates(void);

dbc_GPS_DESTINATION_INFO_s bt__can_update_destination_gps_coordinates(void);
