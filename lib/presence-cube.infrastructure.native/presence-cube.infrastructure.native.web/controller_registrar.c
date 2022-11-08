#include "controller_registrar.h"
// #include "exit_controller.h"
// #include "status_controller.h"
#include "HttpStatusCodes_C.h"

/**
 * @brief Registers endpoints to the live web server
 * @param cancellation_token a token that, if set to true, will signal cancellation to its owner
 */
void controller_registrar__register(
    struct mg_context *webserver_context,
    bool *cancellation_token)
{
    // TODO #141: add /hud endpoint
    // TODO #140: add /led endpoint
    // TODO: use logger instead of direct printf
    printf("initializing controllers (TBD)...\n");
    // exit_controller__register(webserver_context, cancellation_token);
    // status_controller__register(webserver_context, cancellation_token);
}