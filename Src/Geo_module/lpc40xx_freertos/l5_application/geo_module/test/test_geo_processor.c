#include "unity.h"

#include "Mockcmps14_command.h"
#include "Mockcompass.h"
#include "Mockgps.h"

#include "checkpoints.h"
#include "geo_processor.c"
#include "math.h"
#include "project.h"

void test_geo_processor__process_GPS_destination(void) {
  dbc_GPS_DESTINATION_INFO_s incoming_dest_gps_data = {
      .GPS_DESTINATION_LAT = 37.234034,
      .GPS_DESTINATION_LONG = -123.390847,
  };

  geo_processor__process_GPS_destination(&incoming_dest_gps_data);
  TEST_ASSERT_EQUAL_FLOAT(geo_gps_destination.GPS_DESTINATION_LAT, 37.234034);
  TEST_ASSERT_EQUAL_FLOAT(geo_gps_destination.GPS_DESTINATION_LONG, -123.390847);
}

// Origin 1
// 37.335755, -121.881616

// Origin 2
// 37.385550, -121.881485

// Destination 1
// SJSU CAMPUS: East exit of Engr Building on (7th Street)
// 37.336255, -121.881951

/*
  if(1) Using SJSU CAMPUS gps destination settings
    (0) Using Plan B (Gordy Street destination settings)
*/

#if (0)
void test_geo_processor__calculate_destination_bearing_degree(void) {
  gps_coordinates_t test_gps_current = {
      .latitude = 37.335755,
      .longitude = -121.881616,
  };

  dbc_GPS_DESTINATION_INFO_s incoming_dest_gps_data = {
      .GPS_DESTINATION_LAT = 37.336255,
      .GPS_DESTINATION_LONG = -121.881951,
  };

  geo_processor__process_GPS_destination(&incoming_dest_gps_data);

  gps__get_coordinates_ExpectAndReturn(test_gps_current);
  float bearing_in_degrees = geo_processor__calculate_destination_bearing_degree();
  TEST_ASSERT_EQUAL_FLOAT(bearing_in_degrees, 331);
}

void test_geo_processor__calculate_bearing_and_get_compass_data(void) {
  compass_data_t current_heading = {
      .heading_degree_8_bit = 270,
  };
  gps_coordinates_t test_gps_current = {
      .latitude = 37.335755,
      .longitude = -121.881616,
  };

  dbc_GPS_DESTINATION_INFO_s incoming_dest_gps_data = {
      .GPS_DESTINATION_LAT = 37.336255,
      .GPS_DESTINATION_LONG = -121.881951,
  };

  geo_processor__process_GPS_destination(&incoming_dest_gps_data);

  compass__get_data_ExpectAndReturn(current_heading);
  gps__get_coordinates_ExpectAndReturn(test_gps_current);

  geo_processor__get_compass_data();
  TEST_ASSERT_EQUAL_FLOAT(geo_compass_info.CMP_DEST_BEARING, 331);
  TEST_ASSERT_EQUAL_FLOAT(geo_compass_info.CMP_CURRENT_HEADING, 270);
  TEST_ASSERT_EQUAL_FLOAT(geo_compass_info.CMP_DISTANCE, 62.9);
}

// Origin 3 Bottom of Driveway
// 37.385550, -121.881485

// Gordy Street Destination 1
// 37.385723, -121.881087

#else
void test_geo_processor__calculate_destination_bearing_degree(void) {
  gps_coordinates_t test_gps_current = {
      .latitude = 37.385550,
      .longitude = -121.881485,
  };

  dbc_GPS_DESTINATION_INFO_s incoming_dest_gps_data = {
      .GPS_DESTINATION_LAT = 37.385723,
      .GPS_DESTINATION_LONG = -121.881087,
  };

  gps_coordinates_t test_gps_destination = {0};

  geo_processor__process_GPS_destination(&incoming_dest_gps_data);

  gps__get_coordinates_ExpectAndReturn(test_gps_current);
  float bearing_in_degrees = geo_processor__calculate_destination_bearing_degree();
  TEST_ASSERT_EQUAL_FLOAT(bearing_in_degrees, 62);
}

void test_geo_processor__calculate_bearing_and_get_compass_data(void) {
  compass_data_t current_heading = {
      .heading_degree_8_bit = 270,
  };
  gps_coordinates_t test_gps_current = {
      .latitude = 37.385550,
      .longitude = -121.881485,
  };

  dbc_GPS_DESTINATION_INFO_s incoming_dest_gps_data = {
      .GPS_DESTINATION_LAT = 37.385723,
      .GPS_DESTINATION_LONG = -121.881087,
  };

  geo_processor__process_GPS_destination(&incoming_dest_gps_data);

  compass__get_data_ExpectAndReturn(current_heading);
  gps__get_coordinates_ExpectAndReturn(test_gps_current);

  geo_processor__get_compass_data();
  TEST_ASSERT_EQUAL_FLOAT(geo_compass_info.CMP_DEST_BEARING, 62);
  TEST_ASSERT_EQUAL_FLOAT(geo_compass_info.CMP_CURRENT_HEADING, 270);
  TEST_ASSERT_EQUAL_FLOAT(geo_compass_info.CMP_DISTANCE, 19);
  TEST_ASSERT_EQUAL_FLOAT(37.385633, geo_gps_destination.GPS_DESTINATION_LAT);
  TEST_ASSERT_EQUAL_FLOAT(-121.881298, geo_gps_destination.GPS_DESTINATION_LONG);
}

#endif