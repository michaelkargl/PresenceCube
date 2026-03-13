## 1. Domain — Messaging Interface

- [ ] 1.1 Create `presence-cube.domain/messaging/` directory with `library.json` manifest (following the `presence-cube.domain.repositories` pattern — header-only library, no `.c` files)
- [ ] 1.2 Define `message_listener_func.h` with callback typedef `message_listener__on_message_func_t` and function pointer typedefs for initialize, subscribe, start, stop
- [ ] 1.3 Define `message_listener.h` with function declarations: `message_listener__initialize()`, `message_listener__subscribe(topic, callback)`, `message_listener__start()`, `message_listener__stop()`

## 2. Domain — Queuing Interface

- [ ] 2.1 Create `presence-cube.domain/queuing/` directory with `library.json` manifest (header-only, same pattern as messaging)
- [ ] 2.2 Define `command_queue_func.h` with function pointer typedefs for initialize, enqueue, dequeue, destroy
- [ ] 2.3 Define `command_queue.h` with function declarations: `command_queue__initialize(capacity)`, `command_queue__enqueue(payload, length)` returning bool, `command_queue__dequeue(buffer, buffer_size, timeout_ms)` returning int, `command_queue__destroy()`

## 3. Use Case — Message Dispatcher

- [ ] 3.1 Create `presence-cube.usecases/message_dispatch/` directory structure
- [ ] 3.2 Define `message_dispatcher.h` with declarations: `message_dispatcher__initialize()`, `message_dispatcher__initialized()`, `message_dispatcher__run()`, `message_dispatcher__stop()`
- [ ] 3.3 Implement `message_dispatcher.c`: dequeue loop that parses JSON envelope (`type` + `data`), routes `set_led` type to `set_led_command_handler__handle`, logs and discards unknown types and malformed messages, catches CExceptions from handlers
- [ ] 3.4 Add dependency on `presence-cube.domain.queuing` and `cJSON` in `presence-cube.usecases/library.json`

## 4. Infrastructure — ESP32 Command Queue

- [ ] 4.1 Create `presence-cube.esp/command_queue/` directory with `library.json`
- [ ] 4.2 Implement `command_queue.c` using FreeRTOS `xQueueCreate`, `xQueueSend` (non-blocking), `xQueueReceive` (with timeout), `vQueueDelete`

## 5. Infrastructure — ESP32 MQTT Message Listener

- [ ] 5.1 Create `presence-cube.esp/mqtt/` directory with `library.json`
- [ ] 5.2 Implement `mqtt_message_listener.c` using ESP-IDF `esp_mqtt_client`: connect to broker, subscribe to topic, invoke `on_message_callback` with payload on message receipt

## 6. Infrastructure — Native Command Queue

- [ ] 6.1 Create `presence-cube.native/command_queue/` directory with `library.json`
- [ ] 6.2 Implement `command_queue.c` using a circular buffer with pthread mutex and condition variable for thread-safe enqueue/dequeue

## 7. Infrastructure — Native Message Listener

- [ ] 7.1 Add a cross-platform C MQTT client library (e.g., Eclipse Paho MQTT C) as a submodule in `lib/submodules/`, following the existing pattern (cJSON, civetweb)
- [ ] 7.2 Create `presence-cube.native/mqtt/` directory with `library.json`
- [ ] 7.3 Implement `mqtt_message_listener.c` using the MQTT client library: connect to broker at `CONFIG_ESP_MQTT_BROKER_URI`, subscribe to `CONFIG_ESP_MQTT_CONTROL_TOPIC`, invoke callback on message receipt

## 8. Wiring — ESP32 Entry Point

- [ ] 8.1 Add `CUBE_MQTT_ENABLED` feature toggle to `main_esp32.c` (dependent on `CUBE_WIFI_ENABLED`)
- [ ] 8.2 Wire messaging initialization inside the `CUBE_MQTT_ENABLED` guard: initialize command queue, initialize message listener, subscribe to control topic with enqueue callback, initialize dispatcher, start listener, start dispatcher on a dedicated FreeRTOS task

## 9. Wiring — Native Entry Point

- [ ] 9.1 Add `CONFIG_MQTT_ENABLED` toggle to native `sdkconfig.h`
- [ ] 9.2 Wire messaging in `main_native.c` (mirroring ESP32 wiring): initialize command queue, initialize message listener (real MQTT client), subscribe to control topic with enqueue callback, initialize dispatcher, start listener, start dispatcher on a dedicated pthread

## 10. Build Configuration

- [ ] 10.1 Add new library directories to `platformio.ini` `lib_extra_dirs` for both ESP32 and native environments (domain messaging/queuing libraries in shared, platform-specific queue/mqtt in respective sections)

## 11. Testing

- [ ] 11.1 Create unit tests for `message_dispatcher` using Unity + FFF: verify routing of `set_led` messages, handling of unknown types, handling of malformed JSON, handling of handler exceptions
- [ ] 11.2 Create integration test for the native dispatch pipeline: enqueue → dequeue → parse → handle, verifying LED state changes
