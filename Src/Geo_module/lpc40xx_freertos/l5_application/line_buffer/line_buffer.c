#include "line_buffer.h"

/**
 * Initialize *line_buffer_s with the user provided buffer space and size
 * Use should initialize the buffer with whatever memory they need
 * @code
 *  char memory[256];
 *  line_buffer_s line_buffer = { };
 *  line_buffer__init(&line_buffer, memory, sizeof(memory));
 * @endcode
 */
void line_buffer__init(line_buffer_s *buffer, void *memory, size_t size) {
  buffer->memory = memory;
  buffer->max_size = size;
  buffer->write_index = 0;
  buffer->read_index = 0;
  buffer->item_count = 0;
}

bool line_buffer__add_byte(line_buffer_s *buffer, char byte) {
  if (buffer->item_count == buffer->max_size) {
    return false;
  } else {
    ((char *)buffer->memory)[buffer->write_index] = byte;
    buffer->write_index = (buffer->write_index + 1) % buffer->max_size;
    buffer->item_count++;
    return true;
  }
}

bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size) {
  size_t line_index = 0;
  for (int i = 0; i < line_max_size; i++) {
    if (buffer->item_count == 1 && (((char *)buffer->memory)[buffer->read_index]) != '\n') {
      line[line_index] = '\0';
      line_index = (line_index + 1) % line_max_size;
      break;
    } else if ((((char *)buffer->memory)[buffer->read_index]) == '\r') {
      line[line_index] = '\0';
      buffer->read_index = (buffer->read_index + 1) % buffer->max_size;
      buffer->item_count--;
    } else if ((((char *)buffer->memory)[buffer->read_index]) == '\n') {
      line[line_index] = '\0';
      buffer->read_index = (buffer->read_index + 1) % buffer->max_size;
      buffer->item_count--;
      break;
    } else {
      line[line_index] = ((char *)buffer->memory)[buffer->read_index];
      buffer->read_index = (buffer->read_index + 1) % buffer->max_size;
      line_index = (line_index + 1) % line_max_size;
      buffer->item_count--;
    }
  }
  return true;
}
