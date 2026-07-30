# C Design Patterns for Embedded Systems

This repository contains a collection of C Design Patterns for resource-constrained, real-time embedded systems suitable for use with microcontrollers.

These examples all adhere to the following rules:
- **Zero Dynamic Memory Allocation (Heap-less):** No calls to `malloc`/`free` or `calloc`. Memory for handles, context data, and buffers is pre-allocated by the caller (statically at compile-time or on the stack), eliminating heap fragmentation, allocation latency, and memory leak risks.
- **Flash/ROM vs. SRAM Optimization:** Constant configuration tables (state transitions, interface operations tables) are declared `const` so compilers place them directly in Flash/ROM, preserving for runtime data.
- **Fast Bitwise Wrapping:** Wrap-around operations in circular buffers use bitwise AND masking (`&`) instead of integer division modulo (`%`), compiling to single-cycle CPU instructions.
- **Intrusive Linked Lists:** List-based patterns (like the Observer) store linking pointers directly inside the listener objects, maintaining list membership without dynamic node allocation.

## Design Patterns

### 1. State Machine (Finite State Machine, FSM)
- **Location:** [lib/state_machine](./lib/state_machine)
- **Features:** 
  - Application-specific states are decoupled from the library code.
  - Separates the static transition table (`const sm_state_config_t` stored in Flash) from the active state (`sm_handle_t` in RAM).
  - Passes user-defined context pointers (`void *context`) to event callback functions, ensuring reentrancy.
- **Documentation & Usage:** See the header comments in [state_machine.h](./lib/state_machine/state_machine.h).

### 2. Generic Hardware Interface (Polymorphism / VTables)
- **Location:** [lib/interface](./lib/interface)
- **Features:**
  - Implements object-oriented polymorphism in C by using virtual method tables (VTables).
  - Fully generic `intf_t` handle uses a `const void *ops` pointer, allowing arbitrary device classes (e.g., Cameras, Motors, Sensors) to reuse the core interface library.
  - Drivers place their operations tables in Flash (`const`) and context data in RAM, providing clean simulation vs. hardware abstraction for unit testing.
- **Documentation & Usage:** See the header comments in [intf.h](./lib/interface/intf.h) and the concrete camera example in [camera_intf.h](./lib/interface/example/camera_intf.h).

### 3. Circular Ring Buffer
- **Location:** [lib/ring_buffer](./lib/ring_buffer)
- **Features:**
  - A thread-safe, lock-free Single-Producer Single-Consumer (SPSC) FIFO buffer.
  - Requires the buffer size to be a power of two to optimize index wrapping via bitwise masking `(idx + 1) & (max - 1)`.
  - Supports two overflow behaviors: `STOP_ON_FULL` (rejects writes when full) and `OVERWRITE` (overwrites oldest bytes).
- **Documentation & Usage:** See the header comments in [ring_buffer.h](./lib/ring_buffer/ring_buffer.h).

### 4. Observer Pattern
- **Location:** [lib/observer](./lib/observer)
- **Features:**
  - Facilitates loose coupling between event publishers (`obs_subject_t`) and subscribers (`obs_observer_t`).
  - Utilizes intrusive singly-linked lists to register subscribers with zero heap allocations and no static list size limits.
  - Supports event data payloads and instance isolation.
- **Documentation & Usage:** See the header comments in [observer.h](./lib/observer/observer.h).


## Build & Test Instructions

A helper bash script `./build` is provided at the root directory to compile libraries and run their respective unit tests.


## Requirements

* VScode
    * Dev Containers Plugin
* Docker
