## ADDED Requirements

### Requirement: Command queue interface

The domain SHALL define a `command_queue` interface in `presence-cube.domain/queuing/` that abstracts platform-specific queue implementations. The interface SHALL declare:

- `command_queue__initialize(capacity)` — creates a queue with the given capacity
- `command_queue__enqueue(payload, payload_length)` — adds a message payload to the queue (non-blocking, returns success/failure)
- `command_queue__dequeue(buffer, buffer_size, timeout_ms)` — blocks up to `timeout_ms` waiting for the next item, copies payload into buffer, returns actual length (0 if timeout)
- `command_queue__destroy()` — releases queue resources

The interface SHALL NOT reference any specific concurrency mechanism (FreeRTOS, pthreads, etc.).

#### Scenario: Enqueue and dequeue a single message

- **WHEN** a payload is enqueued and then dequeued
- **THEN** the dequeued payload SHALL match the enqueued payload exactly

#### Scenario: Enqueue to a full queue returns failure

- **WHEN** the queue is at capacity and `command_queue__enqueue` is called
- **THEN** the call SHALL return a failure indicator without blocking

#### Scenario: Dequeue from an empty queue blocks until timeout

- **WHEN** the queue is empty and `command_queue__dequeue` is called with a timeout
- **THEN** the call SHALL block for up to `timeout_ms` and return 0 length if no item arrives

#### Scenario: Queue preserves FIFO ordering

- **WHEN** multiple payloads are enqueued in order A, B, C
- **THEN** they SHALL be dequeued in the same order: A, B, C

### Requirement: Command queue function pointer typedefs

The domain SHALL define function pointer typedefs in `command_queue_func.h` for each function in the `command_queue` interface, following the existing `_func.h` pattern. This enables dependency injection and test overriding.

#### Scenario: Function pointer typedefs enable DI for queue operations

- **WHEN** a use case declares a variable of type `command_queue__enqueue_func_t`
- **THEN** it can be assigned any function matching the enqueue signature

### Requirement: ESP32 command queue implementation using FreeRTOS xQueue

The infrastructure layer SHALL provide a FreeRTOS xQueue implementation of `command_queue.h` in `presence-cube.esp/command_queue/`. It SHALL:

- Use `xQueueCreate` for initialization
- Use `xQueueSend` (non-blocking) for enqueue
- Use `xQueueReceive` with timeout for dequeue
- Use `vQueueDelete` for destroy

#### Scenario: ESP32 queue handles concurrent enqueue and dequeue

- **WHEN** the MQTT callback enqueues messages while the dispatcher task dequeues them concurrently
- **THEN** messages SHALL be delivered without corruption or loss (up to queue capacity)

### Requirement: Native command queue implementation using pthread mutex

The infrastructure layer SHALL provide a native implementation of `command_queue.h` in `presence-cube.native/command_queue/` using a circular buffer protected by a pthread mutex and condition variable.

#### Scenario: Native queue supports enqueue and dequeue from different threads

- **WHEN** one thread enqueues and another dequeues concurrently
- **THEN** messages SHALL be delivered correctly without data races
