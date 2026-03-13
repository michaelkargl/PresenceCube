---
id: PC-001
title: Add messaging support
status: To Do
assignee: []
created_date: '2026-03-13 10:08'
labels:
  - feature
  - architecture
milestone: m-0
dependencies: []
references:
  - openspec/changes/add-messaging-support/tasks.md
  - openspec/changes/add-messaging-support/specs/messaging/spec.md
  - openspec/changes/add-messaging-support/specs/queuing/spec.md
  - openspec/changes/add-messaging-support/specs/message-dispatch/spec.md
documentation:
  - openspec/changes/add-messaging-support/proposal.md
  - openspec/changes/add-messaging-support/design.md
priority: high
---

## Description

<!-- SECTION:DESCRIPTION:BEGIN -->
Introduce a transport-agnostic messaging system that enables the cube to receive commands from external sources via MQTT. The design follows the existing onion architecture: domain interfaces for messaging and queuing, a message dispatcher use case that routes typed JSON envelopes to command handlers, and platform-specific infrastructure for both ESP32 (ESP-MQTT + FreeRTOS xQueue) and native (real MQTT client + pthread queue). Both HTTP and MQTT converge on the same command handlers. Initial scope targets LED commands only.
<!-- SECTION:DESCRIPTION:END -->

## Acceptance Criteria
<!-- AC:BEGIN -->
- [ ] #1 The cube can receive set_led commands via MQTT and execute them through the existing set_led_command_handler
- [ ] #2 Message reception is decoupled from command execution via a queue
- [ ] #3 Both ESP32 and native builds connect to a real MQTT broker
- [ ] #4 Messaging is feature-gated and does not affect existing HTTP API behavior
- [ ] #5 Unknown message types and malformed JSON are logged and discarded without crashing
<!-- AC:END -->
