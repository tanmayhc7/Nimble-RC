#include "geo_processor.h"
#include "checkpoints.h"
#include "compass.h"
#include "gps.h"
#include "math.h"

static const float pi_radians = acos(-1);

static dbc_GPS_DESTINATION_INFO_s geo_gps_destination;

static dbc_GPS_CURRENT_INFO_s geo_gps_current;

static dbc_COMPASS_s geo_compass_info;

static gps_coordinates_t gps_current = {0};

static gps_coordinates_t gps_destination = {0};

void geo_processor__process_GPS_destination(dbc_GPS_DESTINATION_INFO_s *gps_destination_info) {
  geo_gps_destination = *gps_destination_info;
}

dbc_COMPASS_s geo_processor__get_compass_data(void) {
  compass_data_t current_heading_degree;
  current_heading_degree = compass__get_data();
  geo_compass_info.CMP_DEST_BEARING = geo_processor__calculate_destination_bearing_degree();
  geo_compass_info.CMP_CURRENT_HEADING = current_heading_degree.heading_degree_8_bit;
  geo_compass_info.CMP_DISTANCE = geo_processor__calculate_distance_between_gps_coordinates();
  return geo_compass_info;
}

static void geo_processor__process_checkpoint_to_geo_gps_destination(void) {
  gps_destination.latitude = geo_gps_destination.GPS_DESTINATION_LAT;
  gps_destination.longitude = geo_gps_destination.GPS_DESTINATION_LONG;
  gps_destination = checkpoints__find_next_point(gps_current, gps_destination);
  geo_gps_destination.GPS_DESTINATION_LAT = gps_destination.latitude;
  geo_gps_destination.GPS_DESTINATION_LONG = gps_destination.longitude;
}

// Destination 1
// SJSU CAMPUS: East exit of Engr Building on (7th Street)
// 37.336255, -121.881951

// Plan B: Destination 2
// Top of Driveway: 37.385588, -121.881561

// Gordy Street Destination 1
// 37.385723, -121.881087

// Gordy Street Destination 2
// 37.385524, -121.881055
// Setting GPS destination without mobile app

float geo_processor__calculate_destination_bearing_degree(void) {
  float my_degree = 0;
  gps_current = gps__get_coordinates();
  geo_gps_current.GPS_CURRENT_LAT = gps_current.latitude;
  geo_gps_current.GPS_CURRENT_LONG = gps_current.longitude;
  geo_gps_destination.GPS_DESTINATION_LAT = 37.385723;
  geo_gps_destination.GPS_DESTINATION_LONG = -121.881087;
  geo_processor__process_checkpoint_to_geo_gps_destination();

  float bearing_x = geo_processor__calculate_bearing_x();
  float bearing_y = geo_processor__calculate_bearing_y();
  my_degree = (float)((atan2(bearing_y, bearing_x) * 180 / (double)pi_radians) + 360);
  return my_degree = (int)(my_degree) % 360;
}

float geo_processor__calculate_bearing_y(void) {
  float bearing_y = (cos((geo_gps_destination.GPS_DESTINATION_LAT * pi_radians / 180))) *
                    (sin((geo_gps_destination.GPS_DESTINATION_LONG * pi_radians / 180) -
                         (geo_gps_current.GPS_CURRENT_LONG * pi_radians / 180)));
  return bearing_y;
}

float geo_processor__calculate_bearing_x(void) {
  float bearing_x = (cos((geo_gps_current.GPS_CURRENT_LAT * pi_radians / 180)) *
                     (sin((geo_gps_destination.GPS_DESTINATION_LAT * pi_radians / 180)))) -
                    (sin((geo_gps_current.GPS_CURRENT_LAT * pi_radians / 180)) *
                     (cos((geo_gps_destination.GPS_DESTINATION_LAT * pi_radians / 180)) *
                      (cos(((geo_gps_destination.GPS_DESTINATION_LONG * pi_radians / 180) -
                            (geo_gps_current.GPS_CURRENT_LONG * pi_radians / 180))))));
  return bearing_x;
}

float geo_processor__calculate_distance_between_gps_coordinates(void) {
  const uint32_t radius_of_earth = 6371000U;

  float latitude_current_in_radians = geo_gps_current.GPS_CURRENT_LAT * pi_radians / 180;
  float latitude_destination_in_radians = geo_gps_destination.GPS_DESTINATION_LAT * pi_radians / 180;

  float delta_latitude = (geo_gps_destination.GPS_DESTINATION_LAT - geo_gps_current.GPS_CURRENT_LAT) * pi_radians / 180;
  float delta_longitude =
      (geo_gps_destination.GPS_DESTINATION_LONG - geo_gps_current.GPS_CURRENT_LONG) * pi_radians / 180;

  float a = sin(delta_latitude / 2) * sin(delta_latitude / 2) + cos(latitude_current_in_radians) *
                                                                    cos(latitude_destination_in_radians) *
                                                                    sin(delta_longitude / 2) * sin(delta_longitude / 2);

  float c = 2 * atan2(sqrt(a), sqrt(1 - a));

  float distance_in_meters = radius_of_earth * c; // in meters

  return floor(distance_in_meters * 10) / 10;
}