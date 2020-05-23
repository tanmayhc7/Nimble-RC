#pragma once
#include "gps.h"

gps_coordinates_t checkpoints__find_next_point(gps_coordinates_t origin, gps_coordinates_t destination);

float checkpoints__calculate_distance_between_coordinates(gps_coordinates_t initial, gps_coordinates_t destination);