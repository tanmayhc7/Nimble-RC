#include "buffer_handler.h"

/******************************************
 *      Private Variables & Funcs
 ******************************************/
static const uint8_t BUFFER_SIZE = 5;
static buffer_S *buf_ptr = NULL;

/******************************************
 *          Init Functions
 ******************************************/
void buf__init(void) {
  buf_ptr = (buffer_S *)malloc(sizeof(buffer_S));
  buf_ptr->iterator_left = 0;
  buf_ptr->iterator_mid = 0;
  buf_ptr->iterator_right = 0;
  buf_ptr->iterator_rear = 0;
}

/******************************************
 *       Add Sensor Value Functions
 ******************************************/
void buf__add_sensor_value_Left(uint16_t push_value) {
  if (buf_ptr->iterator_left == 0) {
    buf_ptr->min_left = push_value;
    buf_ptr->buf_left[buf_ptr->iterator_left++] = push_value;
  } else {
    buf_ptr->min_left = (push_value < buf_ptr->min_left) ? push_value : buf_ptr->min_left;
    buf_ptr->buf_left[buf_ptr->iterator_left++] = push_value;
  }
  if (buf_ptr->iterator_left >= BUFFER_SIZE) {
    buf_ptr->iterator_left = 0;
  }
}

void buf__add_sensor_value_Mid(uint16_t push_value) {
  if (buf_ptr->iterator_mid == 0) {
    buf_ptr->min_mid = push_value;
    buf_ptr->buf_mid[buf_ptr->iterator_mid++] = push_value;
  } else {
    buf_ptr->min_mid = (push_value < buf_ptr->min_mid) ? push_value : buf_ptr->min_mid;
    buf_ptr->buf_mid[buf_ptr->iterator_mid++] = push_value;
  }
  if (buf_ptr->iterator_mid >= BUFFER_SIZE) {
    buf_ptr->iterator_mid = 0;
  }
}

void buf__add_sensor_value_Right(uint16_t push_value) {
  if (buf_ptr->iterator_right == 0) {
    buf_ptr->min_right = push_value;
    buf_ptr->buf_right[buf_ptr->iterator_right++] = push_value;
  } else {
    buf_ptr->min_right = (push_value < buf_ptr->min_right) ? push_value : buf_ptr->min_right;
    buf_ptr->buf_right[buf_ptr->iterator_right++] = push_value;
  }
  if (buf_ptr->iterator_right >= BUFFER_SIZE) {
    buf_ptr->iterator_right = 0;
  }
}

void buf__add_sensor_value_Rear(uint16_t push_value) {
  if (buf_ptr->iterator_rear == 0) {
    buf_ptr->min_rear = push_value;
    buf_ptr->buf_rear[buf_ptr->iterator_rear++] = push_value;
  } else {
    buf_ptr->min_rear = (push_value < buf_ptr->min_rear) ? push_value : buf_ptr->min_rear;
    buf_ptr->buf_rear[buf_ptr->iterator_rear++] = push_value;
  }
  if (buf_ptr->iterator_rear >= BUFFER_SIZE) {
    buf_ptr->iterator_rear = 0;
  }
}

/********************************************
 *          Return Min Value Function
 *********************************************/
uint16_t buf__return_min_value(sensor_e sensor_pos) {
  switch (sensor_pos) {
  case 2:
    return buf_ptr->min_left;
    break;
  case 3:
    return buf_ptr->min_mid;
    break;
  case 4:
    return buf_ptr->min_right;
    break;
  case 5:
    return buf_ptr->min_rear;
    break;
  }
  return 0;
}