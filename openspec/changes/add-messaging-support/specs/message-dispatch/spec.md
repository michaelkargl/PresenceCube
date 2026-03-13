## ADDED Requirements

### Requirement: Message dispatcher use case

The use case layer SHALL provide a `message_dispatcher` in `presence-cube.usecases/message_dispatch/` that:

- Dequeues raw payloads from the command queue
- Parses each payload as a JSON envelope with `type` (string) and `data` (object) fields using cJSON
- Routes to the appropriate command handler based on the `type` field
- Runs in a continuous loop (designed to execute on a dedicated task/thread)

#### Scenario: Dispatcher routes a set_led message to the set_led command handler

- **WHEN** a message with `{ "type": "set_led", "data": { "id": 1, "red": 100, "green": 0, "blue": 0 } }` is dequeued
- **THEN** the dispatcher SHALL parse the data, construct a `set_led_command_t`, and call `set_led_command_handler__handle`

#### Scenario: Dispatcher handles unknown message type

- **WHEN** a message with an unrecognized `type` value is dequeued
- **THEN** the dispatcher SHALL log a warning and discard the message without crashing

#### Scenario: Dispatcher handles malformed JSON

- **WHEN** a message with invalid JSON is dequeued
- **THEN** the dispatcher SHALL log an error and discard the message without crashing

#### Scenario: Dispatcher handles missing envelope fields

- **WHEN** a message is valid JSON but missing the `type` or `data` field
- **THEN** the dispatcher SHALL log an error and discard the message without crashing

#### Scenario: Dispatcher continues processing after a handler error

- **WHEN** a command handler throws a CException (e.g., `ERROR_CODE_RESOURCE_NOT_FOUND` for an invalid LED id)
- **THEN** the dispatcher SHALL catch the exception, log the error, and continue processing the next message

### Requirement: Message dispatcher initialization

The dispatcher SHALL follow the existing handler pattern with:

- `message_dispatcher__initialize()` — sets up dependencies (queue function pointers)
- `message_dispatcher__initialized()` — returns current initialization state
- `message_dispatcher__run()` — starts the dequeue-parse-dispatch loop (blocking, runs until stopped)
- `message_dispatcher__stop()` — signals the dispatch loop to exit

#### Scenario: Dispatcher refuses to run when not initialized

- **WHEN** `message_dispatcher__run()` is called without prior initialization
- **THEN** it SHALL throw `ERROR_CODE_UNINITIALIZED_MODULE_ACCESS`

#### Scenario: Dispatcher stops cleanly when signaled

- **WHEN** `message_dispatcher__stop()` is called while the dispatch loop is running
- **THEN** the loop SHALL exit after completing the current message (if any)

### Requirement: Messaging wiring in ESP32 entry point

The ESP32 `main_esp32.c` SHALL initialize and start messaging components when `CUBE_MQTT_ENABLED` is defined:

1. Initialize the command queue
2. Initialize the message listener (ESP-MQTT)
3. Subscribe to the control topic with a callback that enqueues raw payloads
4. Initialize the message dispatcher
5. Start the message listener
6. Start the dispatcher on a dedicated FreeRTOS task

#### Scenario: ESP32 messaging components are feature-gated

- **WHEN** `CUBE_MQTT_ENABLED` is set to 0
- **THEN** no messaging components SHALL be initialized or started

#### Scenario: ESP32 messaging starts after WiFi and LED initialization

- **WHEN** `CUBE_MQTT_ENABLED` is set to 1
- **THEN** messaging initialization SHALL occur after WiFi connection and LED store initialization are complete

### Requirement: Messaging wiring in native entry point

The native `main_native.c` SHALL initialize and start messaging components when `CONFIG_MQTT_ENABLED` is defined, mirroring the ESP32 wiring:

1. Initialize the command queue
2. Initialize the message listener (native MQTT client)
3. Subscribe to the control topic with a callback that enqueues raw payloads
4. Initialize the message dispatcher
5. Start the message listener
6. Start the dispatcher on a dedicated thread

The native build is a functional replica of the embedded device and SHALL connect to a real MQTT broker.

#### Scenario: Native messaging components are feature-gated

- **WHEN** `CONFIG_MQTT_ENABLED` is not set
- **THEN** no messaging components SHALL be initialized or started

#### Scenario: Native messaging connects to real MQTT broker

- **WHEN** the native build runs with messaging enabled
- **THEN** the MQTT client SHALL connect to the broker at `CONFIG_ESP_MQTT_BROKER_URI` and subscribe to `CONFIG_ESP_MQTT_CONTROL_TOPIC`

#### Scenario: Native messaging processes real MQTT messages

- **WHEN** an MQTT message is published to the control topic on the broker
- **THEN** the native build SHALL receive, dequeue, dispatch, and handle the message identically to the ESP32 build
