# Add Messaging Support

## Problem

The PresenceCube is currently controllable only via HTTP web API. The project's original vision includes receiving commands from cloud infrastructure pushed to the device. There is no inbound messaging capability today — only request/response via HTTP controllers.

## Vision

The cube will eventually be controlled exclusively via a cloud digital twin:

```
┌────────┐  (1) desired   ┌────────────────┐  (3) per-LED    ┌────────┐
│  Cube  │─────state──────▶│  Cloud Twin     │────commands────▶│  Cube  │
│        │                 │                 │                 │        │
│        │                 │  (2) calculates │                 │  (4)   │
│        │                 │  delta per LED  │                 │  sets  │
└────────┘                 └────────────────┘                 │  LEDs  │
                                                              └────────┘
```

The twin sends multiple commands asynchronously — one per LED — which the cube must process reliably without blocking message reception.

## Proposal

Introduce a transport-agnostic messaging system that enables the cube to receive commands from external sources (initially MQTT, but not limited to it). The design follows the existing onion architecture:

- **Domain layer**: A `message_listener` interface — the domain knows "I can receive messages" without knowing the transport protocol. A `command_queue` interface — the domain knows "I can enqueue and dequeue commands" without knowing the concurrency mechanism.
- **Use case layer**: A `message_dispatch` application service that receives typed message envelopes, determines the command type, parses the data payload, and routes to the appropriate command handler.
- **Infrastructure layer**: Platform-specific implementations of the message listener (ESP-MQTT for ESP32, a lightweight implementation for native development) and the command queue (FreeRTOS xQueue for ESP32, native equivalent for desktop).

Incoming messages are enqueued immediately upon arrival, keeping the MQTT client responsive. A separate consumer dequeues commands and dispatches them sequentially through the use case handlers. This decoupling is necessary because future command types (display/HUD) involve blocking SPI operations that would stall message reception if executed inline.

The initial scope targets the LED use case only — dispatching `set_led` commands via messaging. The HUD use case requires its own refactoring effort and will be handled separately.

## Architecture

### Layered Overview

```
┌──────────────────────────────────────────────────────────────────────────┐
│  INFRASTRUCTURE (platform-specific, thin)                                 │
│                                                                           │
│  presence-cube.esp/                    presence-cube.native/              │
│  ┌──────────────────────┐              ┌──────────────────────┐           │
│  │ mqtt/                 │              │ mqtt/                 │           │
│  │ ESP-MQTT client       │              │ native impl           │           │
│  │ implements             │              │ implements             │           │
│  │ message_listener.h    │              │ message_listener.h    │           │
│  └───────────┬───────────┘              └───────────┬───────────┘           │
│              │ on message                           │ on message            │
│              ▼                                      ▼                      │
│  ┌──────────────────────┐              ┌──────────────────────┐           │
│  │ command_queue/        │              │ command_queue/        │           │
│  │ FreeRTOS xQueue       │              │ native queue impl     │           │
│  │ implements             │              │ implements             │           │
│  │ command_queue.h       │              │ command_queue.h       │           │
│  └───────────┬───────────┘              └───────────┬───────────┘           │
│              │ dequeue                              │ dequeue               │
└──────────────┼──────────────────────────────────────┼──────────────────┘
               │                                      │
               ▼                                      ▼
┌──────────────────────────────────────────────────────────────────────────┐
│  USE CASES (presence-cube.usecases/)                                      │
│                                                                           │
│  message_dispatch/               set_led/              get_led/           │
│  ┌──────────────────────┐       ┌──────────────┐      ┌──────────────┐   │
│  │ dequeues command       │       │ command       │      │ query         │   │
│  │ determines type        │──────▶│ handler       │      │ handler       │   │
│  │ parses → command_t     │       └──────────────┘      └──────────────┘   │
│  │ dispatches to handler  │                                                │
│  └────────────────────────┘                                                │
└──────────────────────────────────────────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────────────────────────┐
│  DOMAIN (presence-cube.domain/)                                           │
│                                                                           │
│  messaging/                              led/  repositories/  hal/        │
│    message_listener.h                                                     │
│    message_listener_func.h                                                │
│  queuing/                                                                 │
│    command_queue.h                                                        │
│    command_queue_func.h                                                   │
└──────────────────────────────────────────────────────────────────────────┘
```

### Message Format

All messages use a typed envelope with a `type` field for routing and a `data` field for command-specific content:

```json
{
  "type": "set_led",
  "data": {
    "id": 1,
    "red": 100,
    "green": 0,
    "blue": 0
  }
}
```

The message listener can subscribe to any number of transport-specific topics (e.g. `control/leds`, `control/display`), but the dispatcher only cares about the `type` field — transport details are fully erased before dispatch:

```
MQTT topic: control/leds         ─┐
MQTT topic: control/display      ─┤    ┌───────────────────────┐
MQTT topic: control/whatever     ─┼───▶│ message listener      │
future transport X               ─┘    │ strips transport,     │
                                        │ enqueues raw payload  │
                                        └───────────┬───────────┘
                                                    │
                                                    │  { "type": "...", "data": { ... } }
                                                    ▼
                                        ┌───────────────────────┐
                                        │ dispatcher            │
                                        │ reads "type"          │
                                        │ routes to handler     │
                                        └───────────────────────┘
```

### Message Flow

```
  MQTT message arrives
         │
         ▼
  ┌──────────────┐
  │ message      │   infrastructure: receives bytes,
  │ listener     │   enqueues immediately, returns
  └──────┬───────┘
         │ enqueue(raw payload)
         ▼
  ┌──────────────┐
  │ command      │   infrastructure: platform-specific
  │ queue        │   queue implementation
  └──────┬───────┘
         │ dequeue (separate task/thread)
         ▼
  ┌──────────────┐
  │ message      │   use case: read "type",
  │ dispatcher   │   parse "data", route to handler
  └──────┬───────┘
         │
         ▼
  ┌──────────────┐
  │ set_led      │   use case: execute
  │ handler      │   command
  └──────────────┘
```

Both HTTP and MQTT converge on the same command handlers. HTTP bypasses the queue (synchronous request/response). MQTT goes through the queue (async, decoupled):

```
  ┌──────────────┐     ┌──────────────┐
  │  HTTP / Web  │     │    MQTT      │
  │  Controller  │     │  Subscriber  │
  └──────┬───────┘     └──────┬───────┘
         │                    │
         │ direct             │ via queue
         │                    │
         ▼                    ▼
  ┌──────────────────────────────────┐
  │      Use Case Handlers           │
  │  set_led_command_handler         │
  └──────────────────────────────────┘
```

### File Structure

```
presence-cube.domain/
  messaging/
    message_listener.h            → interface (initialize, subscribe, stop)
    message_listener_func.h       → function pointer typedefs (on_message callback)
  queuing/
    command_queue.h               → interface (enqueue, dequeue, initialize)
    command_queue_func.h          → function pointer typedefs

presence-cube.usecases/
  message_dispatch/
    message_dispatcher.h          → dispatch API
    message_dispatcher.c          → dequeue, parse payload, route to handler

presence-cube.esp/mqtt/
  mqtt_message_listener.c        → ESP-MQTT implementation of message_listener.h
presence-cube.esp/command_queue/
  command_queue.c                 → FreeRTOS xQueue implementation of command_queue.h

presence-cube.native/mqtt/
  mqtt_message_listener.c        → native implementation of message_listener.h
presence-cube.native/command_queue/
  command_queue.c                 → native implementation of command_queue.h
```

## Scope

- Transport-agnostic messaging interface in the domain
- Command queue interface in the domain with platform-specific implementations
- Message dispatcher as a shared application service in use cases (LED commands only)
- ESP32 MQTT client implementation (receive-only, subscribe to control topic)
- Native messaging implementation (for development without hardware)
- Wire messaging and queue initialization into both platform entry points (`main_esp32.c`, `main_native.c`)

## Non-goals

- Publishing/sending messages from the device (data topic — future work)
- MQTT QoS tuning or advanced broker configuration
- Replacing the existing HTTP API — both transports coexist
- Native parity with a full MQTT broker (a simpler mechanism suffices)
- HUD command dispatching (requires separate refactoring of `hud_controller.c`)

## Decisions

- **Message format**: Typed envelope (`type` + `data`) — transport-agnostic, extensible, dispatcher routes on `type` only.
- **Queue over direct dispatch**: Future command types (HUD/display) involve blocking SPI operations. The queue decouples message reception from command execution, keeping the MQTT client responsive.
- **Messaging interface in domain**: Transport-agnostic `message_listener` — the domain defines "I can receive messages" without referencing MQTT or any specific protocol.
- **Dispatcher in use cases**: Application service that routes typed envelopes to command handlers. Shared across platforms — infrastructure is a dumb pipe.
