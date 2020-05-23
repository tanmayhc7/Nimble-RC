#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "checkpoints.h"
#include "geo_processor.h"

static const float pi_radians = acos(-1);
static const float max_allowed_distance_in_meters = 1000.0;
static const float minimum_checkpoint_arrival_distance = 3.0;

/*
 *   If(1) - Checkpoints with SJSU Campus Checkpoints
 *   If(0) - Checkpoints with Gordy Street
 */

#if (0)
// static const gps_coordinates_t locations_we_can_travel[] = {
//     {37.336055, -121.882217}, // Checkpoint 1
//     {37.336198, -121.881903}, // Checkpoint 2
//     {37.335915, -121.881748}, // Checkpoint 3
//     {37.335678, -121.881582}, // Checkpoint 4
//     {37.335293, -121.881323}, // Checkpoint 5
//     {37.334934, -121.881034}, // Checkpoint 6
//     {37.334692, -121.880835}, // Checkpoint 7
//     {37.334600, -121.880652}, // Checkpoint 8
//     {37.334801, -121.880208}, // Checkpoint 9
//     {37.334986, -121.879815}, // Checkpoint 10
//     {37.335096, -121.879573}, // Checkpoint 11
// };

Gordy Street Destination 1
    // 37.385723, -121.881087

    Gordy Street Destination 2
    // 37.385524, -121.881055

    Gordy Street Origin Test 1
// 37.385561, -121.881391

#else
static const gps_coordinates_t locations_we_can_travel[] = {
    {37.385633, -121.881298}, // Checkpoint 1
    {37.385685, -121.881164}, // Checkpoint 2
    {37.385798, -121.881169}, // Checkpoint 3
    {37.385575, -121.881076}, // Checkpoint 4
};

#endif

    gps_coordinates_t
    checkpoints__find_next_point(gps_coordinates_t origin, gps_coordinates_t destination) {
  float distance_from_origin_to_checkpoint = 0;
  float distance_from_checkpoint_to_destination = 0;
  float nearest_checkpoint_distance = max_allowed_distance_in_meters;
  float nearest_destination_distance = max_allowed_distance_in_meters;
  uint8_t next_check_point_index = 0;
  bool check_point_found = false;
  float distance_from_origin_to_destination = checkpoints__calculate_distance_between_coordinates(origin, destination);
  printf("\ndistance from origin to destination: %f\n", distance_from_origin_to_destination);

  uint8_t size_of_checkpoints = sizeof(locations_we_can_travel) / sizeof(locations_we_can_travel[0]);
  for (int i = 0; i < size_of_checkpoints; i++) {
    distance_from_origin_to_checkpoint =
        checkpoints__calculate_distance_between_coordinates(origin, locations_we_can_travel[i]);
    printf("\ndistance from origin to checkpoint: %f\n", distance_from_origin_to_checkpoint);
    distance_from_checkpoint_to_destination =
        checkpoints__calculate_distance_between_coordinates(locations_we_can_travel[i], destination);
    printf("distance from checkpoint to destination: %f\n", distance_from_checkpoint_to_destination);
    if ((distance_from_origin_to_checkpoint < nearest_checkpoint_distance) &&
        (distance_from_origin_to_checkpoint > minimum_checkpoint_arrival_distance) &&
        (distance_from_checkpoint_to_destination <= distance_from_origin_to_destination)) {
      nearest_checkpoint_distance = distance_from_origin_to_checkpoint;
      nearest_destination_distance = distance_from_checkpoint_to_destination;
      printf("nearest checkpoint distance: %f \n", nearest_checkpoint_distance);
      printf("nearest destination distance: %f \n", nearest_destination_distance);
      next_check_point_index = i;
      check_point_found = true;
    } else if ((distance_from_origin_to_checkpoint <= minimum_checkpoint_arrival_distance) &&
               check_point_found == true) {
      next_check_point_index = i - 1;
      // break;
    } else if (distance_from_origin_to_checkpoint <= minimum_checkpoint_arrival_distance &&
               check_point_found == false) {
      if (distance_from_checkpoint_to_destination == distance_from_origin_to_destination &&
          check_point_found == false) {
        return destination;
      } else
        next_check_point_index = i + 1;
      // break;
    }
  }
  return locations_we_can_travel[next_check_point_index];
}

float checkpoints__calculate_distance_between_coordinates(gps_coordinates_t initial, gps_coordinates_t destination) {
  const uint32_t radius_of_earth = 6371000U;

  float latitude_current_in_radians = initial.latitude * pi_radians / 180;
  float latitude_destination_in_radians = destination.latitude * pi_radians / 180;

  float delta_latitude = (destination.latitude - initial.latitude) * pi_radians / 180;
  float delta_longitude = (destination.longitude - initial.longitude) * pi_radians / 180;

  float a = sin(delta_latitude / 2) * sin(delta_latitude / 2) + cos(latitude_current_in_radians) *
                                                                    cos(latitude_destination_in_radians) *
                                                                    sin(delta_longitude / 2) * sin(delta_longitude / 2);

  float c = 2 * atan2(sqrt(a), sqrt(1 - a));

  float distance_in_meters = radius_of_earth * c; // in meters

  return floor(distance_in_meters * 10) / 10;
}