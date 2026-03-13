## Context

The PresenceCube is an ESP32-based IoT device currently controllable only via HTTP request/response. The project follows an onion architecture with clean separation between domain (interfaces), use cases (command/query handlers), and infrastructure (platform-specific implementations for ESP32 and native desktop).

The existing patterns are:
- **Domain interfaces**: Header files (`.h`) with function declarations + `_func.h` files with function pointer typedefs for dependency injection
- **Use case handlers**: Command/query DTOs with `handler__handle(command_t) → response_t` pattern
- **Infrastructure**: Platform-specific `.c` files that implement domain `.h` interfaces directly
- **DI wiring**: Use case `.c` files hold function pointer variables initialized with default implementations; tests override via `extern`
- **Error handling**: CException with typed error codes and THROW macros
- **JSON**: cJSON library with parse utilities and serialization helpers
- **Build**: PlatformIO with `lib_extra_dirs` separating platform-specific code; each library has a `library.json` manifest

This change introduces inbound messaging to enable cloud-to-device command delivery, initially via MQTT.

## Goals / Non-Goals

**Goals:**
- Enable the cube to receive commands asynchronously via messaging (initially MQTT)
- Decouple message reception from command execution via a queue to keep the MQTT client responsive
- Reuse existing command handlers — both HTTP and MQTT converge on the same `set_led_command_handler`
- Follow existing architecture patterns exactly: domain interfaces, use case handlers, platform-specific infrastructure
- Support both ESP32 (ESP-MQTT + FreeRTOS xQueue) and native (real MQTT client + pthread queue) as a functional replica

**Non-Goals:**
- Publishing/sending messages from the device
- MQTT QoS tuning or advanced broker configuration
- Replacing the HTTP API
- HUD command dispatching (separate effort)
- Native-only shortcuts — the native build is a functional replica of the embedded device

## Decisions

### 1. Message listener as a domain interface

**Decision**: Define `message_listener.h` and `message_listener_func.h` in `presence-cube.domain/messaging/`, following the same pattern as `get_led_repository.h` / `get_led_repository_func.h`.

**Rationale**: The domain knows "I can receive messages" without knowing the transport. This keeps MQTT (or any future transport) entirely in infrastructure. The `_func.h` pattern enables DI in use cases and test overriding via `extern`, consistent with existing code.

**Alternative considered**: Defining the listener interface in use cases. Rejected because the concept of "receiving messages" is a domain concern — it describes a capability of the system, not application-specific orchestration.

### 2. Command queue as a domain interface

**Decision**: Define `command_queue.h` and `command_queue_func.h` in `presence-cube.domain/queuing/`, with platform-specific implementations in infrastructure.

**Rationale**: The queue decouples message reception from command execution. Future command types (HUD/display) involve blocking SPI operations that would stall the MQTT client if executed inline. The domain defines the queueing contract; infrastructure provides the concurrency mechanism (FreeRTOS xQueue on ESP32, a simple mutex-guarded queue on native).

**Alternative considered**: Direct dispatch without a queue. Rejected because the MQTT callback runs in the MQTT task context on ESP32 — blocking it with SPI operations would cause message loss and watchdog timeouts.

### 3. Typed JSON envelope as the message format

**Decision**: All messages use `{ "type": "...", "data": { ... } }`. The dispatcher routes on `type` only — transport details (topics, QoS) are fully erased before dispatch.

**Rationale**: Transport-agnostic, extensible, and simple to parse with cJSON. Adding new command types requires only a new `type` string and handler — no changes to the messaging infrastructure.

**Alternative considered**: Using MQTT topics to determine command type. Rejected because it couples routing logic to transport, and the domain should not know about MQTT topics.

### 4. Message dispatcher as a use case

**Decision**: `message_dispatcher.h/.c` in `presence-cube.usecases/message_dispatch/`. It dequeues from the command queue, parses the JSON envelope, and routes to the appropriate command handler based on `type`.

**Rationale**: The dispatcher is application-level orchestration — it coordinates between the queue (domain interface) and command handlers (use cases). This is the same layer where `set_led_command_handler` lives. The dispatcher uses the existing cJSON parsing patterns and THROW macros for error handling.

**Alternative considered**: Placing dispatch logic in infrastructure. Rejected because routing logic is not platform-specific — it's shared application behavior.

### 5. ESP32: ESP-MQTT client + FreeRTOS xQueue

**Decision**: Use the ESP-IDF built-in `esp_mqtt_client` for MQTT and `xQueueCreate`/`xQueueSend`/`xQueueReceive` for the command queue. A dedicated FreeRTOS task runs the dispatch loop.

**Rationale**: ESP-MQTT is included in ESP-IDF (no external dependency). FreeRTOS queues are the standard ESP32 mechanism for inter-task communication — thread-safe, ISR-compatible, and well-documented.

### 6. Native: real MQTT client + pthread mutex queue

**Decision**: Implement the command queue using a simple array-based circular buffer protected by a pthread mutex. For MQTT, use a cross-platform C MQTT client library (e.g., Eclipse Paho MQTT C) added as a submodule, following the existing pattern for third-party libraries (cJSON, civetweb). The native build connects to the same broker as ESP32 using the existing config defines (`CONFIG_ESP_MQTT_BROKER_URI`, `CONFIG_ESP_MQTT_CONTROL_TOPIC`, etc.) from `sdkconfig.h`.

**Rationale**: The native build is a functional replica of the embedded device, not a mock. Using a real MQTT client against a real broker ensures the full message pipeline — broker → listener → queue → dispatcher → handler — is exercised identically on both platforms. This catches integration issues early without requiring hardware.

### 7. Feature-gated initialization

**Decision**: Add `CUBE_MQTT_ENABLED` feature toggle in both entry points, following the existing pattern (`CUBE_WIFI_ENABLED`, `CUBE_WEBSERVER_ENABLED`). MQTT depends on WiFi on ESP32.

**Rationale**: Consistent with the existing feature toggle pattern. Allows enabling/disabling messaging independently of the HTTP API.

## Risks / Trade-offs

**[Queue overflow under message burst]** → The command queue has a fixed capacity. If the cloud twin sends more commands than the dispatcher can process, messages will be dropped. Mitigation: size the queue to handle the expected burst (number of LEDs × commands per LED) and log drops.

**[JSON parsing overhead per message]** → Every message is parsed from JSON on the device. For per-LED commands this means N parses for N LEDs. Mitigation: cJSON is lightweight and the payloads are small (~50 bytes). Profile if performance becomes an issue; a binary format can be introduced later behind the same envelope abstraction.

**[Native implementation divergence]** → The native queue uses pthreads while ESP32 uses FreeRTOS. Behavioral differences in timing, blocking, and error semantics could mask bugs. Mitigation: the domain interface is the contract — integration tests on both platforms verify the same scenarios through the same dispatcher.

**[MQTT connection lifecycle]** → The initial implementation does not handle reconnection, broker unavailability, or network transitions. Mitigation: ESP-MQTT handles reconnection internally. This is acceptable for the initial scope; robustness improvements can follow.
