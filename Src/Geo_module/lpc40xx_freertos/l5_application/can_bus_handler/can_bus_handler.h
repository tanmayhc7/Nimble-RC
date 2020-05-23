#pragma once

void can_bus_handler__init(void);

void can_bus_handler_bus_off(void);

void can_bus_handler__transmit_messages_10Hz(void);

void can_bus_handler__mia_handle_10Hz(void);

void can_bus_handler__process_all_received_messages_10Hz(void);