#include "unity.h"

#include "checkpoints.h"

static gps_coordinates_t test_origin = {0};

static gps_coordinates_t test_destination = {0};

void setUp(void) {}

void tearDown(void) {}

// SJSU starting point (origin 1)
// 37.335358, -121.881366

// SJSU Front of School of Music and Dance (origin 2)
// 37.335390, -121.881367

// Destination 1
// SJSU CAMPUS: East exit of Engr Building on (7th Street)
// 37.336255, -121.881951

// Destination 2
// SJSU CAMPUS: Walk way between Event Center and New Gym(far).
// 37.335039, -121.879714

// {37.336055, -121.882217}, // Checkpoint 1
// {37.336198, -121.881903}, // Checkpoint 2
// {37.335915, -121.881748}, // Checkpoint 3
// {37.335678, -121.881582}, // Checkpoint 4
// {37.335293, -121.881323}, // Checkpoint 5
// {37.334934, -121.881034}, // Checkpoint 6
// {37.334692, -121.880835}, // Checkpoint 7
// {37.334600, -121.880652}, // Checkpoint 8
// {37.334801, -121.880208}, // Checkpoint 9
// {37.334986, -121.879815}, // Checkpoint 10
// {37.335096, -121.879573}, // Checkpoint 11

// Gordy Street Destination 1
// 37.385723, -121.881087

// Gordy Street Origin Test 1
// 37.385561, -121.881391

// Checkpoints for Gordy Street Test
// {37.385633, -121.881298}, // Checkpoint 1
// {37.385685, -121.881164}, // Checkpoint 2
// {37.385798, -121.881169}, // Checkpoint 3
// {37.385575, -121.881076}, // Checkpoint 4

/*
 *   If(1) - Unit-Test Checkpoints with SJSU Campus Checkpoints
 *   If(0) - Unit-Test Checkpoints with Gordy Street
 */

// void test_checkpoints_get_next_point_for_destination_1(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.335358,
//       .longitude = -121.881366,
//   };
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.336255,
//       .longitude = -121.881951,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.335678, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.881582, (double)test_checkpoint.longitude);
// }

// void test_checkpoints_get_next_point_start_from_checkpoint_4_to_3_for_destination_1(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start origin at checkpoint 4
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.335678,
//       .longitude = -121.881582,
//   };
//   // Destination at East exit of Engr building
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.336255,
//       .longitude = -121.881951,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.335915, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.881748, (double)test_checkpoint.longitude);
// }

// void test_checkpoints_get_next_point_start_from_checkpoint_3_to_2_for_destination_1(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start origin at checkpoint 3
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.335915,
//       .longitude = -121.881748,
//   };
//   // Destination at East exit of Engr building
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.336255,
//       .longitude = -121.881951,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.336198, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.881903, (double)test_checkpoint.longitude);
// }
// // 37.336198, -121.881903 checkpoint 2
// // 37.336255, -121.881951 destination 1
// void test_checkpoints_get_next_point_start_from_checkpoint_2_to_destination_1(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start origin at checkpoint 2
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.336198,
//       .longitude = -121.881903,
//   };
//   // Destination at East exit of Engr building
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.336255,
//       .longitude = -121.881951,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.336255, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.881951, (double)test_checkpoint.longitude);
// }

// // Test set 2 for origin 2(School of Music and Dance)
// // 37.335390, -121.881367
// // {37.335293, -121.881323}, // Checkpoint 5

// void test_checkpoints_get_next_point_start_from_origin_2_to_checkpoint_5_for_destination_2(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start origin 2
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.335390,
//       .longitude = -121.881367,
//   };
//   // Destination 2 between Event Center and New Gym(far)
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.335039,
//       .longitude = -121.879714,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.335293, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.881323, (double)test_checkpoint.longitude);
// }

// void test_checkpoints_get_next_point_start_checkpoint_5_to_6_for_destination_2(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start at checkpoint 5
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.335293,
//       .longitude = -121.881323,
//   };
//   // Destination 2 between Event Center and New Gym(far)
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.335039,
//       .longitude = -121.879714,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.334934, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.881034, (double)test_checkpoint.longitude);
// }

// void test_checkpoints_get_next_point_start_checkpoint_6_to_7_for_destination_2(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start at checkpoint 6
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.334934,
//       .longitude = -121.881034,
//   };
//   // Destination 2 between Event Center and New Gym(far)
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.335039,
//       .longitude = -121.879714,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.334692, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.880835, (double)test_checkpoint.longitude);
// }

// // {37.334692, -121.880835}, // Checkpoint 7
// // {37.334600, -121.880652}, // Checkpoint 8

// void test_checkpoints_get_next_point_start_checkpoint_7_to_8_for_destination_2(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start at checkpoint 7
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.334692,
//       .longitude = -121.880835,
//   };
//   // Destination 2 between Event Center and New Gym(far)
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.335039,
//       .longitude = -121.879714,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.334600, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.880652, (double)test_checkpoint.longitude);
// }

// // {37.334600, -121.880652}, // Checkpoint 8
// // {37.334801, -121.880208}, // Checkpoint 9

// void test_checkpoints_get_next_point_start_checkpoint_8_to_9_for_destination_2(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start at checkpoint 8
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.334600,
//       .longitude = -121.880652,
//   };
//   // Destination 2 between Event Center and New Gym(far)
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.335039,
//       .longitude = -121.879714,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.334801, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.880208, (double)test_checkpoint.longitude);
// }

// // {37.334801, -121.880208}, // Checkpoint 9
// // {37.334986, -121.879815}, // Checkpoint 10

// void test_checkpoints_get_next_point_start_checkpoint_9_to_10_for_destination_2(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start at checkpoint 9
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.334801,
//       .longitude = -121.880208,
//   };
//   // Destination 2 between Event Center and New Gym(far)
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.335039,
//       .longitude = -121.879714,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.334986, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.879815, (double)test_checkpoint.longitude);
// }

// // {37.334986, -121.879815}, // Checkpoint 10
// void test_checkpoints_get_next_point_start_checkpoint_10_to_destination_2(void) {
//   gps_coordinates_t test_checkpoint = {0};
//   // Start at checkpoint 10
//   static gps_coordinates_t test_origin = {
//       .latitude = 37.334986,
//       .longitude = -121.879815,
//   };
//   // Destination 2 between Event Center and New Gym(far)
//   static gps_coordinates_t test_destination = {
//       .latitude = 37.335039,
//       .longitude = -121.879714,
//   };

//   test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
//   TEST_ASSERT_EQUAL_FLOAT(37.335039, (double)test_checkpoint.latitude);
//   TEST_ASSERT_EQUAL_FLOAT(-121.879714, (double)test_checkpoint.longitude);
// }

// Gordy Street Destination 1
// 37.385723, -121.881087

// Gordy Street Origin Test 1
// 37.385561, -121.881391

// Checkpoints for Gordy Street Test
// {37.385633, -121.881298}, // Checkpoint 1
// {37.385685, -121.881164}, // Checkpoint 2
// {37.385798, -121.881169}, // Checkpoint 3
// {37.385575, -121.881076}, // Checkpoint 4

void test_checkpoints_gordy_street_origin_to_checkpoint_1_for_destination_1(void) {
  gps_coordinates_t test_checkpoint = {0};
  static gps_coordinates_t test_origin = {
      .latitude = 37.385561,
      .longitude = -121.881391,
  };
  static gps_coordinates_t test_destination = {
      .latitude = 37.385723,
      .longitude = -121.881087,
  };

  test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
  TEST_ASSERT_EQUAL_FLOAT(37.385633, (double)test_checkpoint.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.881298, (double)test_checkpoint.longitude);
}

// {37.385633, -121.881298}, // Checkpoint 1

void test_checkpoints_gordy_street_checkpoint_1_to_2(void) {
  gps_coordinates_t test_checkpoint = {0};
  static gps_coordinates_t test_origin = {
      .latitude = 37.385633,
      .longitude = -121.881298,
  };
  static gps_coordinates_t test_destination = {
      .latitude = 37.385723,
      .longitude = -121.881087,
  };

  test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
  TEST_ASSERT_EQUAL_FLOAT(37.385685, (double)test_checkpoint.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.881164, (double)test_checkpoint.longitude);
}

void test_checkpoints_gordy_street_checkpoint_2_to_destination_1(void) {
  gps_coordinates_t test_checkpoint = {0};
  static gps_coordinates_t test_origin = {
      .latitude = 37.385685,
      .longitude = -121.881164,
  };
  static gps_coordinates_t test_destination = {
      .latitude = 37.385723,
      .longitude = -121.881087,
  };

  test_checkpoint = checkpoints__find_next_point(test_origin, test_destination);
  TEST_ASSERT_EQUAL_FLOAT(37.385723, (double)test_checkpoint.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.881087, (double)test_checkpoint.longitude);
}