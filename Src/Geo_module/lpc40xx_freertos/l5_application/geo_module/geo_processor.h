#pragma once

#include "project.h"

void geo_processor__process_GPS_current(dbc_GPS_CURRENT_INFO_s *gps_current_info);

void geo_processor__process_GPS_destination(dbc_GPS_DESTINATION_INFO_s *gps_destination_info);

dbc_COMPASS_s geo_processor__get_compass_data(void);

float geo_processor__calculate_destination_bearing_degree(void);

float geo_processor__calculate_bearing_y(void);

float geo_processor__calculate_bearing_x(void);

float geo_processor__calculate_distance_between_gps_coordinates(void);
