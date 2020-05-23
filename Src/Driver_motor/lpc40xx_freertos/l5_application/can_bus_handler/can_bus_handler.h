#pragma once

void can_bus_handler__init(void);

void can_bus_handler__handle_bus_off(void);

void can_bus_handler__process_all_received_messages(void);

void can_bus_handler__transmit_messages(void);

void can_bus_handler__mia_handle_10hz(void);

void can_bus_handler__transmit_driver_debug_can_init(void);