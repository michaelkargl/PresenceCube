#pragma once
#include "log_level.h"

/**
 * @brief logs an info message.
 *        Do not use within an interrupt.
 * 
 * @param context_name identifies the context of the log entry
 * @param message_format a printf format string
 * @param ... variadic / variable list of message arguments
 * @see printf
 */
void log_information(const char* context_name, const char* message_format, ...);

/**
 * @brief logs a debug message
 * 
 * @param context_name identifies the context of the log entry
 * @param message_format a printf format string
 * @param ... variadic / variable list of message arguments
 * @see printf
 */
void log_debug(const char* context_name, const char* message_format, ...);

/**
 * @brief logs a warning message
 * 
 * @param context_name identifies the context of the log entry
 * @param message_format a printf format string
 * @param ... variadic / variable list of message arguments
 * @see printf
 */
void log_warning(const char* context_name, const char* message_format, ...);


/**
 * @brief logs an error message
 * 
 * @param context_name identifies the context of the log entry
 * @param message_format a printf format string
 * @param ... variadic / variable list of message arguments
 * @see printf
 */
void log_error(const char* context_name, const char* message_format, ...);
