#pragma once
#include <stdbool.h>

void driver_node__mia_handle_10hz(void);

void driver_node__handle_messages_over_can(void);

bool driver_node__transmit_motor_cmd_over_can(void);