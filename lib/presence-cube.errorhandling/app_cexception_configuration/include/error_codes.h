#pragma once

/**
 * @brief define all possible error codes and error messages here.
 * @see https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-
 */
typedef enum {
    ERROR_CODE_SUCCESS = 0x0000,

    ERROR_CODE_INDEX_OUT_OF_RANGE = 0x000A,
    ERROR_CODE_LOWER_BOUNDS_BIGGER_THAN_UPPER_BOUNDS = 0x000B,

    ERROR_CODE_UNINITIALIZED_DATA_ACCESS = 0x00A0,
    ERROR_CODE_ARGUMENT_NULL = 0x00AA,

    ERROR_CODE_CALL_NOT_IMPLEMENTED = 0x0078,
    ERROR_CODE_INSUFFICIENT_BUFFER = 0x007A,

    ERROR_CODE_UNHANDLED = 0xFFFF
} error_code_t;
