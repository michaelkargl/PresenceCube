#include "json_parse__set_led_request_dto_t.h"
#include "json_parse_utils.h"
#include "CException.h"


struct set_led_request_dto_t json_parse__set_led_request_dto(cJSON *json)
{
    // TODO: test
    return (struct set_led_request_dto_t){
        .id = get_number_property(json, "id")->valueint,
        .red = get_number_property(json, "r")->valueint,
        .green = get_number_property(json, "g")->valueint,
        .blue = get_number_property(json, "b")->valueint};
}

