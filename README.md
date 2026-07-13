# C Design Patterns

This is a collection of design patterns for embedded C. This includes GoogleTest for verification and usage examples.

* Callback (aka Observer)
* Interface (aka Hardware Abstration Layer, HAL)
* Ring Buffer (aka Circular Buffer)
* State Machine (aka Finite State Machine, FSM)


## Build Commands

`Lib Dir` is the name of the library directory in lib/.

* **./build test `Lib Dir`**
    * Compiles with debug flags and runs test.
* **./build debug `Lib Dir`**
    * Compiles with debug flags.


## Container Packages

* vim
* gcc
* gdb
* make
* cmake


## Requirements

If you have GCC 11 and GDB installed locally, you should be able to run on the host machine.

* VScode
    * Dev Containers Plugin
* Docker
