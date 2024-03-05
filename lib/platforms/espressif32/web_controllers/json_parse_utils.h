#pragma once
#include "cJSON.h"

/**
 * @brief Extracts the given property from a cJSON object
 * @param json the json object from which to extract the @ref property_name
 * @return cJSON* a reference to the requested json object
 * @exception ERROR_CODE_RESOURCE_NOT_FOUND if json property could not be found
 * @exception ERROR_CODE_INVALID_TYPE if json property is not a number
 * @exception ERROR_CODE_ARGUMENT_NULL if input object is null
 */
cJSON *get_number_property(cJSON *object, const char *const property_name);