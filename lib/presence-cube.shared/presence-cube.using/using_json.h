#pragma once
#include "using_func.h"
#include "cJSON.h"

typedef CJSON_PUBLIC(void) (*cJSON_Delete_func_t)(cJSON *item);

static cJSON_Delete_func_t _using_json__cJSON_Delete_func = cJSON_Delete;

/**
 * @brief frees given JSON document on exception or 
 *        after successful execution of provided code block.
 */
#define USING_JSON(variable, code_block) USING_FUNC(variable, code_block, _using_json__cJSON_Delete_func);