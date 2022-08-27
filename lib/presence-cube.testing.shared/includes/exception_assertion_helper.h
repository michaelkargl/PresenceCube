#pragma once
#include "unity.h"
#include "CException.h"

/**
 * @brief Executes the given code block and asserts if it catches the expected exception
 * @param exception the exception to be listening for
 * @param code_block logic enclosed in parenthesis that is to be executed.
 *                   IMPORTANT: Since the error handling lib needs to release resources,
 *                              DO NOT prematurely return, jump or break out of this block.
 */
#define TEST_ASSERT_THROWS(exception, code_block)                                                       \
    do                                                                                                  \
    {                                                                                                   \
        volatile CEXCEPTION_T ex = ERROR_CODE_SUCCESS;                                                  \
        Try                                                                                             \
        {                                                                                               \
            do                                                                                          \
                code_block while (0);                                                                   \
            TEST_PRINTF("Expected " #code_block " to throw %x-exception, but none caught.", exception); \
            TEST_FAIL();                                                                                \
        }                                                                                               \
        Catch(ex) {}                                                                                    \
        TEST_ASSERT_EQUAL_HEX_MESSAGE(exception, ex, "Exceptions do not match.");                           \
    } while (0);

/**
 * @brief Executes the given code block and asserts if it runs without errors.
 * @param code_block logic enclosed in parenthesis that is to be executed.
 *                   IMPORTANT: Since the error handling lib needs to release resources,
 *                              DO NOT prematurely return, jump or break out of this block.
 */
#define TEST_ASSERT_THROWS_NOT(code_block)             \
    do                                                 \
    {                                                  \
        volatile CEXCEPTION_T ex = ERROR_CODE_SUCCESS; \
        Try                                            \
        {                                              \
            do                                         \
                code_block while (0);                  \
        }                                              \
        Catch(ex) {}                                   \
                                                       \
        TEST_ASSERT_EQUAL(ERROR_CODE_SUCCESS, ex);     \
    } while (0);

// eof
