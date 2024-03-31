#pragma once
#include "stddef.h"
#include "error_codes.h"
#include "logger.iface.h"
#include "CException.h"

/**
 * @brief Logs and throws given exception.
 * @param message_printf_format exception message format string in the printf format.
 * @param ... variadic message format arguments.
 */
#define THROW(exception, message_printf_format, ...)                  \
    LOG_ERROR(message_printf_format "\n" __VA_OPT__(, ) __VA_ARGS__); \
    Throw(exception);

/**
 * @brief Throws given exception if condition is TRUE.
 * @param condition A condition that resolves to a boolean.
 * @param message_printf_format exception message format string in the printf format.
 * @param ... variadic message format arguments.
 * @example THROW_IF_TRUTHY(
 *              ERROR_CODE_RESOURCE_NOT_FOUND,
 *              entity == NULL,
 *              "entity %u not found",
 *              id);
 */
#define THROW_IF_TRUTHY(exception, condition, message_printf_format, ...)        \
    if (condition)                                                               \
    {                                                                            \
        THROW(exception, message_printf_format "\n" __VA_OPT__(, ) __VA_ARGS__); \
    }

/**
 * @brief Throws given exception if condition evaluates to FALSE.
 * @param condition A condition that resolves to a boolean.
 * @param message_printf_format exception message format string in the printf format.
 * @param ... variadic message format arguments.
 * @example THROW_IF_FALSY(
 *              ERROR_CODE_UNINITIALIZED,
 *              _initialized(),
 *              "Illegal access to uninitialized module.");
 */
#define THROW_IF_FALSY(exception, condition, message_printf_format, ...) THROW_IF_TRUTHY( \
    exception,                                                                            \
    !(condition),                                                                         \
    message_printf_format "\n" __VA_OPT__(, ) __VA_ARGS__);

/**
 * @brief Throws given exception if reference is NULL.
 * @param message_printf_format exception message format string in the printf format.
 * @param ... variadic message format arguments.
 */
#define THROW_IF_NULL(exception, variable, message_printf_format, ...) THROW_IF_TRUTHY( \
    exception,                                                                          \
    variable == NULL,                                                                   \
    message_printf_format "\n" __VA_OPT__(, ) __VA_ARGS__);

/**
 * @brief Throws @ref ERROR_CODE_RESOURCE_NOT_FOUND if reference is NULL.
 * @param message_printf_format exception message format string in the printf format.
 * @param ... variadic message format arguments.
 */
#define THROW_RESOURCE_NOT_FOUND_IF_NULL(variable, message_printf_format, ...) \
    THROW_IF_NULL(                                                             \
        ERROR_CODE_RESOURCE_NOT_FOUND,                                         \
        variable,                                                              \
        message_printf_format "\n" __VA_OPT__(, ) __VA_ARGS__);

/**
 * @brief Throws if the given reference is null.
 * @param variable The variable to test for nullability.
 */
#define THROW_ARGUMENT_NULL_IF_NULL(variable) THROW_IF_TRUTHY( \
    ERROR_CODE_ARGUMENT_NULL,                                  \
    variable == NULL,                                          \
    "ArgumentNullException: Argument '" #variable "' cannot be null.\n");

/**
 * @brief Throws if the given structure contains a falsy `is_initialized` member.
 * @param variable The variable to test for it's initialization state.
 */
#define THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT(variable)                                     \
    if (!variable.is_initialized)                                                                        \
    {                                                                                                    \
        log_error(__func__, "UninitializedAccessException: value of '" #variable "' cannot be null.\n"); \
        Throw(ERROR_CODE_UNINITIALIZED_DATA_ACCESS);                                                     \
    };

/**
 * @brief Throws if the given structure reference is NULL or contains a falsy `is_initialized` member.
 * @param variable The variable to test for it's initialization state.
 */
#define THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(variable)                                                         \
    do                                                                                                                           \
    {                                                                                                                            \
        bool uninitialized = variable == NULL || !variable->is_initialized;                                                      \
        if (uninitialized)                                                                                                       \
        {                                                                                                                        \
            log_error(__func__, "UninitializedAccessException: reference '" #variable "' must not be null or uninitialized.\n"); \
            Throw(ERROR_CODE_UNINITIALIZED_DATA_ACCESS);                                                                         \
        };                                                                                                                       \
    } while (0);

// eof
