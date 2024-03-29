#pragma once
#include "error_codes.h"

// TODO: fix naming: <module>__<function>()

/**
 * @brief reverts changes made by its counter part: uncaught_error_handler_init
 */
void uncaught_error_handler_deinit();

/**
 * @brief initializes default error handling
 * @param handle_uncaught_error_callback this function acts as a circuit breaker
 * in case uncaught exceptions were being detected. At this point, there
 * is a very high probability of memory leaks already present. It is
 * advised to either free those resources or abort execution alltogether.
 */
void uncaught_error_handler_init(
    void (*handle_uncaught_error_callback)(error_code_t)
);

/**
 * @brief this function acts as a circuit breaker
 * in case uncaught exceptions were being detected. At this point, there
 * is a very high probability of memory leaks already present. It is
 * advised to either free those resources or abort execution alltogether. 
 * @param error_code_t error_code that has been thrown
 */
void uncaught_error_handler_handle(error_code_t error_code_t);