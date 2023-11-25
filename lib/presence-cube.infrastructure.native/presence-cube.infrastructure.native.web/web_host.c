#include "web_host.h"
#include "civetweb.h"
#include "stdio.h"
#include "controller_registrar.h"
#include "string.h"
#include <unistd.h>
#include "logger.h"

#define WEBSERVER_SLEEP_INTERVAL_SECONDS 1
#define MAX_DOTS_PER_ROW 60
#define LOGGING_DISABLED 1
#define LOGGING_ENABLED 0

static struct mg_callbacks _event_callbacks;
static struct mg_context *_webserver_context = NULL;

static int log_message(const struct mg_connection *conn, const char *message)
{
    LOG_DEBUG("[_log_message]: %s", message);
    return LOGGING_ENABLED;
}

static void register_event_callbacks()
{
    LOG_DEBUG("registering event callbacks...");
    memset(&_event_callbacks, 0, sizeof(_event_callbacks));
    _event_callbacks.log_message = log_message;
    _event_callbacks.log_access = log_message;
}

static void initialize_webserver()
{
    LOG_DEBUG("initializing webserver...");
    mg_init_library(MG_FEATURES_DEFAULT);
    register_event_callbacks();
}


static void stop_webserver()
{
    LOG_DEBUG("stopping web server...\n");
    if (_webserver_context == NULL)
    {
        return;
    }

    mg_stop(_webserver_context);
    LOG_DEBUG("Server stopped.\n");
    LOG_DEBUG("Bye!\n");
}

static bool start_webserver(const char *webserver_options[])
{
    LOG_DEBUG("Starting web server...\n");
    _webserver_context = mg_start(&_event_callbacks, NULL, webserver_options);
    return _webserver_context != NULL;
}

static void await_cancellation(bool *_cancellation_token)
{
    // blocking the root thread from closing
    // wait until cancellation has been requested
    for (__uint8_t iteration_counter = 1; !*_cancellation_token; iteration_counter++)
    {
        fprintf(stdout, ".");
        bool full_row = iteration_counter % MAX_DOTS_PER_ROW == 0;
        if (full_row)
        {
            fprintf(stdout, "\n");
            iteration_counter = 0;
        }
        fflush(stdout);
        fflush(stderr);
        sleep(WEBSERVER_SLEEP_INTERVAL_SECONDS);
    }
}

int web_host__host_web_application(
    const char *webserver_options[],
    int (*application_fn)(bool *),
    bool *cancellation_token)
{
    printf("Starting web host...\n");   

    initialize_webserver();
    if (!start_webserver(webserver_options))
    {
        fprintf(stderr, "Unable to start web server instance...\n");
        return 1;
    }
    
    // Endpoint registration requires a running web server
    controller_registrar__register(_webserver_context, cancellation_token);
    
    application_fn(cancellation_token);
    await_cancellation(cancellation_token);

    stop_webserver();
    return 0;
}