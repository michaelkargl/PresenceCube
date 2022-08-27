#include "json_parse_utils.h"
#include "CException.h"

cJSON *get_number_property(cJSON *object, const char *const property_name)
{
    THROW_ARGUMENT_NULL_IF_NULL(object);
    THROW_ARGUMENT_NULL_IF_NULL(property_name);

    cJSON *property = cJSON_GetObjectItemCaseSensitive(object, property_name);
    
    if (property == NULL) {
        THROW(ERROR_CODE_RESOURCE_NOT_FOUND, "JSON property '%s' could not be found.", property_name);
    }

    if (!cJSON_IsNumber(property))
    {
        THROW(ERROR_CODE_INVALID_TYPE, "JSON property '%s' is not a number.", property_name);
    }

    return property;
}