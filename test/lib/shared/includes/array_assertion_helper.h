#pragma once

#define ARRAY_LENGTH(array_stack_ref) (sizeof(array_stack_ref) / sizeof(array_stack_ref[0]))

#define TEST_ASSERT_EVERY_INITIALIZED(array, array_length_uint32)                                              \
    do                                                                                                         \
    {                                                                                                          \
        TEST_ASSERT_NOT_NULL(array);                                                                           \
        for (uint32_t index = 0; index < array_length_uint32; index++)                                         \
        {                                                                                                      \
            bool initialized = array[index].is_initialized;                                                    \
            if (!initialized)                                                                                  \
            {                                                                                                  \
                TEST_PRINTF("Expected element " #array "[%i] to be initialized, but it was not.", index);      \
                TEST_FAIL_MESSAGE("Expected all elements of " #array " to be initialized but some were not."); \
            }                                                                                                  \
        }                                                                                                      \
    } while (0)

// eof
