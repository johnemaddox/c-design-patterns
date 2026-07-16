# C Design Patterns for Embedded Systems

This repository contains a collection of production-grade, highly optimized C design patterns tailored specifically for resource-constrained, real-time embedded systems. Each pattern is fully documented, decoupled, reentrant (supports multiple instances), and verified using GoogleTest.

---

## Core Optimization Principles

To suit limited-resource microcontrollers (e.g., ARM Cortex-M, AVR, RISC-V), the patterns adhere to the following principles:
- **Zero Dynamic Memory Allocation (Heap-less):** No calls to `malloc`/`free` or `calloc`. Memory for handles, context data, and buffers is pre-allocated by the caller (statically at compile-time or on the stack), eliminating heap fragmentation, allocation latency, and memory leak risks.
- **Flash/ROM vs. SRAM Optimization:** Constant configuration tables (e.g., state transitions, interface operations tables) are declared `const` so compilers place them directly in Flash/ROM, preserving precious SRAM for runtime data.
- **Fast Bitwise Wrapping:** Wrap-around operations in circular buffers use bitwise AND masking (`&`) instead of integer division modulo (`%`), compiling to single-cycle CPU instructions.
- **Intrusive Linked Lists:** List-based patterns (like the Observer/Callback) store linking pointers directly inside the listener objects, maintaining list membership without dynamic node allocation.

---

## Design Patterns

### 1. State Machine (Finite State Machine, FSM)
- **Location:** [lib/state_machine](file:///home/john/workspaces/c-design-patterns/lib/state_machine)
- **Features:** 
  - Application-specific states are decoupled from the library code.
  - Separates the static transition table (`const sm_state_config_t` stored in Flash) from the active state (`sm_handle_t` in RAM).
  - Passes user-defined context pointers (`void *context`) to event callback functions, ensuring reentrancy.
- **Documentation & Usage:** See the header comments in [state_machine.h](file:///home/john/workspaces/c-design-patterns/lib/state_machine/state_machine.h).

### 2. Generic Hardware Interface (Polymorphism / VTables)
- **Location:** [lib/interface](file:///home/john/workspaces/c-design-patterns/lib/interface)
- **Features:**
  - Implements object-oriented polymorphism in C by using virtual method tables (VTables).
  - Fully generic `intf_t` handle uses a `const void *ops` pointer, allowing arbitrary device classes (e.g., Cameras, Motors, Sensors) to reuse the core interface library.
  - Drivers place their operations tables in Flash (`const`) and context data in RAM, providing clean simulation vs. hardware abstraction for unit testing.
- **Documentation & Usage:** See the header comments in [intf.h](file:///home/john/workspaces/c-design-patterns/lib/interface/intf.h) and the concrete camera example in [camera_intf.h](file:///home/john/workspaces/c-design-patterns/lib/interface/example/camera_intf.h).

### 3. Circular Ring Buffer
- **Location:** [lib/ring_buffer](file:///home/john/workspaces/c-design-patterns/lib/ring_buffer)
- **Features:**
  - A thread-safe, lock-free Single-Producer Single-Consumer (SPSC) FIFO buffer.
  - Requires the buffer size to be a power of two to optimize index wrapping via bitwise masking `(idx + 1) & (max - 1)`.
  - Supports two overflow behaviors: `STOP_ON_FULL` (rejects writes when full) and `OVERWRITE` (overwrites oldest bytes).
- **Documentation & Usage:** See the header comments in [ring_buffer.h](file:///home/john/workspaces/c-design-patterns/lib/ring_buffer/ring_buffer.h).

### 4. Callback (Observer Pattern)
- **Location:** [lib/callback](file:///home/john/workspaces/c-design-patterns/lib/callback)
- **Features:**
  - Facilitates loose coupling between event publishers (`cb_subject_t`) and subscribers (`cb_observer_t`).
  - Utilizes intrusive singly-linked lists to register subscribers with zero heap allocations and no static list size limits.
  - Supports event data payloads and instance isolation.
- **Documentation & Usage:** See the header comments in [callback.h](file:///home/john/workspaces/c-design-patterns/lib/callback/callback.h).

---

## Build & Test Instructions

A helper bash script `./build` is provided at the root directory to compile libraries and run their respective unit tests.

### Commands

* **Compile and run unit tests for a library:**
  ```bash
  ./build test <lib_dir>
  ```
  *Example:*
  ```bash
  ./build test ring_buffer
  ```

* **Compile with debug flags without running tests:**
  ```bash
  ./build debug <lib_dir>
  ```

Replace `<lib_dir>` with one of the directory names in `lib/` (e.g., `state_machine`, `interface`, `ring_buffer`, or `callback`).

---

## Requirements

* VScode
    * Dev Containers Plugin
* Docker
