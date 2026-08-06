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

### 2. Hardware Interface (Polymorphism / VTables)
- **Location:** [lib/interface](./lib/interface)
- **Features:**
  - Implements object-oriented polymorphism in C by using virtual method tables (VTables).
  - Fully generic `intf_handle_t` handle uses a `const void *ops` pointer, allowing arbitrary device classes (e.g., Cameras, Motors, Sensors) to reuse the core interface library.
  - Drivers place their operations tables in Flash (`const`) and context data in RAM, providing clean simulation vs. hardware abstraction for unit testing.
- **Documentation & Usage:** See the header comments in [intf.h](./lib/interface/intf.h) and the concrete camera example in [camera_intf.h](./lib/interface/example/camera_intf.h).

### 3. Circular Ring Buffer
- **Location:** [lib/ring_buffer](./lib/ring_buffer)
- **Features:**
  - A thread-safe, lock-free Single-Producer Single-Consumer (SPSC) FIFO buffer.
  - Requires the buffer size to be a power of two to optimize index wrapping via bitwise masking `(idx + 1) & (max - 1)`.
  - Supports two overflow behaviors: `STOP_ON_FULL` (rejects writes when full) and `OVERWRITE` (overwrites oldest bytes).
- **Documentation & Usage:** See the header comments in [ring_buffer.h](./lib/ring_buffer/ring_buffer.h).

### 4. Callback Patterns
The repository implements three callback patterns optimized for resource-constrained systems:

* **Stateful Reentrant Callback:**
  - **Location:** [lib/callback](./lib/callback)
  - **Features:** Decouples driver logic from application context. Pairs function pointers with generic `void *context` pointers to support reentrancy without relying on global variables.
  - **Documentation & Usage:** See the header comments in [callback.h](./lib/callback/callback.h) and unit tests in [callback-test.cpp](./tests/callback-test.cpp).

* **Operations Table Callback (VTable):**
  - **Location:** [lib/callback_table](./lib/callback_table)
  - **Features:** Groups related callback function pointers into a single operations struct stored in Flash (`const ct_table_t *`). A single pointer in the RAM handle references the table, saving critical SRAM bytes when dealing with multiple event handlers per instance.
  - **Documentation & Usage:** See [callback_table.h](./lib/callback_table/callback_table.h) and unit tests in [callback_table-test.cpp](./tests/callback_table-test.cpp).

* **Queue-Deferred Callback (ISR-Safe):**
  - **Location:** [lib/callback_queue](./lib/callback_queue)
  - **Features:** Integrates with a Single-Producer Single-Consumer (SPSC) ring buffer (reusing [lib/ring_buffer](./lib/ring_buffer)) to serialize callback execution structures. This allows fast, deterministic interrupts (ISRs) to post callback structures to a queue, deferring execution to thread/task context.
  - **Documentation & Usage:** See [callback_queue.h](./lib/callback_queue/callback_queue.h) and unit tests in [callback_queue-test.cpp](./tests/callback_queue-test.cpp).


## Build & Test Instructions

A helper bash script `./build` is provided at the root directory to compile libraries and run their respective unit tests. Run without arguments for help.


## Requirements

* VScode
    * Dev Containers Plugin
* Docker
