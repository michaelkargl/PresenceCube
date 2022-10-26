#pragma once
#include "exception_handling.h"
#include "cJSON.h"

/**
 * @brief calls given function in the following cases:
 *       1._after_ successful execution of the provided code block
 *       2. if an exception is caught (in which case the exception is rethrown)
 * @see https://github.com/ThrowTheSwitch/CException/#return--goto
 * @param variable a reference variable to be passed to @ref func
 * @param code_block a block of code to execute before calling @ref func.
 *                   Due to limitations in our error handling strategy, prematurely
 *                   breaking the execution of @ref code_block by means other than a @ref Throw
 *                   statement (for instance: return or goto), is unsupported. This would
 *                   bypass certain clean-ups steps, causing memory leaks or unpredicted behavior.
 * @param func a function that receives @ref variable as input parameter
 */
#define USING_FUNC(variable, code_block, func) \
    volatile CEXCEPTION_T ex;                  \
    Try                                        \
    {                                          \
        do                                     \
            code_block while (0);              \
        func(variable);                        \
    }                                          \
    Catch(ex)                                  \
    {                                          \
        func(variable);                        \
        Throw(ex);                             \
    }

// eof
