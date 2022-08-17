#pragma once

/**
 * @brief define all possible error codes and error messages here.
 */

typedef enum {
    ERROR_CODE_SUCCESS = 0x0000,

    INDEX_OUT_OF_RANGE = 0x000A,
    LOWER_BOUNDS_BIGGER_THAN_UPPER_BOUNDS = 0x000B,

    UNINITIALIZED_DATA_ACCESS_ERROR = 0x00A0,

    ERROR_CODE_UNHANDLED = 0xFFFF
} error_code_t;
