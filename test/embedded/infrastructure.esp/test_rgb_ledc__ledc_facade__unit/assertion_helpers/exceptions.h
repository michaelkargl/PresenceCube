#pragma once
#include "unity.h"
#include "CException.h"

#define TEST_ASSERT_THROWS(exception, code_block)                              \
    do                                                                         \
    {                                                                          \
        volatile CEXCEPTION_T ex;                                              \
        Try                                                                    \
        {                                                                      \
            do                                                                 \
                code_block while (0);                                          \
            TEST_PRINTF("Expected exception %i, but none caught.", exception); \
            TEST_FAIL();                                                       \
        }                                                                      \
        Catch(ex) {}                                                           \
                                                                               \
        TEST_ASSERT_EQUAL_MESSAGE(exception, ex, "Exceptions do not match.");  \
    } while (0);

// eof
