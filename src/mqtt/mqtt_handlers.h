#pragma once

// groups together mqtt event handlers
typedef struct {
    void (*on_message_received)(const char *);
    void (*on_connected)();
} mqtt_handlers_t;
