#pragma once
#include <stdbool.h>

void geo_node__mia_handle_10Hz(void);

void geo_node__handle_can_messages_10Hz(void);

bool geo_node__transmit_can_messages_10Hz(void);