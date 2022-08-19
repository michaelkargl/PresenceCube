#pragma once
#include "error_codes.h"
#include "logger.h"

// TODO: Unit Test !!!

#define THROW_ARGUMEN_NULL(variable)                                                  \
    do                                                                                \
    {                                                                                 \
        log_error("ArgumentNullException: Argument %s cannot be null.\n", #variable); \
        Throw(ERROR_CODE_ARGUMENT_NULL);                                              \
    } while (0)

/**
 * @brief Throws if the given reference is null.
 */
#define THROW_ARGUMENT_NULL_IF_NULL(variable) \
    if (variable == NULL)                     \
    {                                         \
        THROW_ARGUMEN_NULL(variable);         \
    };

/**
 * @brief Throws if the given structure contains a falsy `is_initialized` member.
 */
#define THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT(variable)                      \
    if (!variable.is_initialized)                                                         \
    {                                                                                     \
        log_error("UninitializedAccessException: value %s cannot be null.\n", #variable); \
        Throw(ERROR_CODE_ARGUMENT_NULL);                                                  \
    };

/**
 * @brief Throws if the given structure reference is NULL or contains a falsy `is_initialized` member.
 */
#define THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_REFERENCE(variable)                                              \
    do                                                                                                               \
    {                                                                                                                \
        bool is_null = variable == NULL;                                                                             \
        bool uninitialized = !variable->is_initialized;                                                              \
        if (is_null || uninitialized)                                                                                \
        {                                                                                                            \
            log_error("UninitializedAccessException: reference %s must not be null or uninitialized.\n", #variable); \
            Throw(ERROR_CODE_UNINITIALIZED_DATA_ACCESS);                                                             \
        };                                                                                                           \
    } while (0);

// eof
