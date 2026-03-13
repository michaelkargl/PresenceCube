## ADDED Requirements

### Requirement: Message listener interface

The domain SHALL define a `message_listener` interface in `presence-cube.domain/messaging/` that abstracts transport-specific message reception. The interface SHALL declare:

- `message_listener__initialize()` — prepares the listener for operation
- `message_listener__subscribe(topic, on_message_callback)` — registers a callback for a given topic
- `message_listener__start()` — begins listening for messages
- `message_listener__stop()` — stops listening and releases resources

The interface SHALL NOT reference any specific transport protocol (MQTT, HTTP, etc.).

#### Scenario: Listener receives a message and invokes callback

- **WHEN** the message listener is initialized, subscribed to a topic, and started
- **THEN** incoming messages on that topic SHALL be delivered to the registered `on_message_callback` with the raw payload as a byte buffer and its length

#### Scenario: Listener supports multiple topic subscriptions

- **WHEN** the listener subscribes to multiple topics
- **THEN** messages on each topic SHALL be delivered to the corresponding callback

#### Scenario: Listener stop prevents further message delivery

- **WHEN** `message_listener__stop()` is called
- **THEN** no further callbacks SHALL be invoked for any subscribed topic

### Requirement: Message listener function pointer typedefs

The domain SHALL define function pointer typedefs in `message_listener_func.h` for each function in the `message_listener` interface, following the existing `_func.h` pattern (e.g., `get_led_repository_func.h`). This enables dependency injection and test overriding via `extern`.

#### Scenario: Function pointer typedefs enable DI in use cases

- **WHEN** a use case declares a variable of type `message_listener__subscribe_func_t`
- **THEN** it can be assigned any function matching the subscribe signature, enabling platform-specific or test implementations

### Requirement: On-message callback signature

The domain SHALL define the callback signature as a function pointer typedef: `typedef void (*message_listener__on_message_func_t)(const char *payload, int payload_length)`. The callback receives the raw message payload without transport metadata.

#### Scenario: Callback receives raw payload without transport details

- **WHEN** a message arrives on a subscribed topic
- **THEN** the callback SHALL receive only the payload bytes and length — no topic name, QoS, or transport-specific metadata

### Requirement: ESP32 MQTT message listener implementation

The infrastructure layer SHALL provide an ESP-MQTT implementation of `message_listener.h` in `presence-cube.esp/mqtt/`. It SHALL:

- Use the ESP-IDF `esp_mqtt_client` API
- Connect to the broker configured via build-time settings
- Subscribe to configured MQTT topics
- On message receipt, invoke the registered callback with the payload

#### Scenario: ESP32 listener connects and subscribes to MQTT topic

- **WHEN** `message_listener__initialize()` and `message_listener__start()` are called on ESP32
- **THEN** the ESP-MQTT client SHALL connect to the configured broker and subscribe to the configured topic(s)

#### Scenario: ESP32 listener delivers MQTT message payload to callback

- **WHEN** an MQTT message is received on a subscribed topic
- **THEN** the registered `on_message_callback` SHALL be invoked with the message payload and length

### Requirement: Native MQTT message listener implementation

The infrastructure layer SHALL provide a native implementation of `message_listener.h` in `presence-cube.native/mqtt/` using a cross-platform C MQTT client library (e.g., Eclipse Paho MQTT C). It SHALL:

- Connect to the broker configured via `CONFIG_ESP_MQTT_BROKER_URI` in `sdkconfig.h`
- Subscribe to configured MQTT topics (`CONFIG_ESP_MQTT_CONTROL_TOPIC`)
- On message receipt, invoke the registered callback with the payload
- Function as a real MQTT client, identical in behavior to the ESP32 implementation

#### Scenario: Native listener connects and subscribes to MQTT topic

- **WHEN** `message_listener__initialize()` and `message_listener__start()` are called on native
- **THEN** the MQTT client SHALL connect to the configured broker and subscribe to the configured topic(s)

#### Scenario: Native listener delivers MQTT message payload to callback

- **WHEN** an MQTT message is received on a subscribed topic
- **THEN** the registered `on_message_callback` SHALL be invoked with the message payload and length
