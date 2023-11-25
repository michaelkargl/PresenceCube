#pragma once
#include "using_func.h"
#include "stdlib.h"

typedef void (*free_func_t)(void *);

free_func_t _using_free__free_func = free;

/**
 * @brief Some variables are supposed to be `const` within a code_block, however, when
 *        passing `const void*` as `void*` to free, the compiler explains the const will be
 *        ignored and, depending on compiler flags, fail build execution. This function
 *        explicitly tells the compiler by casting, that this is expected behavior.
 */
static void free_const(const void *variable)
{
    _using_free__free_func((void *)variable);
}

/**
 * @brief frees given variable on exception or after successful execution of provided code block.
 * @example const char* text = generate_string();
 *          USING(text, {
 *              // process
 *          })
 */
#define USING(variable, code_block) USING_FUNC(variable, code_block, free_const);

// eof