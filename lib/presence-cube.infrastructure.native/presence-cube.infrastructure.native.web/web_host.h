#pragma once
#include "stdbool.h"

/**
 * @brief Starts a web server and provides a host for a function to run
 * @param webserver_options options to pass to the web server (port, timeout, ...)
 * @param application_fn a pointer to the applications function
 * @param _cancellation_token a pointer to a token to signal the wish to cancel 
 *                            the currently running instruction.
 * @return 0 if the cann was successful
 * @return >0 if it was unsuccessful
 */
int web_host__host_web_application(
    const char *webserver_options[],
    int (*application_fn)(bool *),
    bool *_cancellation_token);