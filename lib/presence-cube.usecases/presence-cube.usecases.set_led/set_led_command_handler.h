#pragma once
#include "set_led_command_response.h"
#include "set_led_command.h"
#include "stdbool.h"

/**
 * @brief Initializes the command handlers internal state.
 */
void set_led_command_handler__initialize();

/**
 * @brief Returns the modules' current initialization state
 * @return true initialized
 * @return false not initialized
 */
bool set_led_command_handler__initialized();

/**
 * @brief Handles a state change request for a given LED
 * 
 * @param command A structure representing the desire to modify the current state of a LED
 * @return set_led_command_response_t
 * @see https://github.com/jbogard/MediatR/wiki#requestresponse
 */
set_led_command_response_t set_led_command_handler__handle(const set_led_command_t command);