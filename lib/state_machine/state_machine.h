/*H************************************************************
 *
 * Finite State Machine (FSM)
 *
 *  Provides a resource-efficient, generic finite state machine library.
 *  Decoupled from application-specific states, allowing multiple
 *  independent state machines to run concurrently.
 *
 *  To minimize RAM consumption in resource-constrained embedded systems:
 *   - The state transition tables (sm_state_config_t) are declared const
 *     to reside in Flash/ROM.
 *   - The active state and context (sm_handle_t) reside in RAM.
 *
 *  Supports reentrancy and instanced operations by passing the handle
 *  and user-defined context pointers back to event callback functions.
 *
 *  Usage Example:
 *   1. Define states and event callbacks.
 *   2. Declare a static const sm_state_config_t table[] in Flash.
 *   3. Declare an sm_handle_t in RAM pointing to the table.
 *   4. Run state transitions periodically by calling sm_run(&handle).
 *
 * @author John E Maddox
 *
 * @version 2.0.0
 *
 *************************************************************H*/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stddef.h>
#include <stdint.h>

// Generic type for state IDs
typedef uint32_t sm_state_t;

typedef struct sm_handle_t sm_handle_t;

// State configuration  (stored in Flash/ROM by using the const keyword in sm_handle_t below)
typedef struct
{
    sm_state_t (*event)(sm_handle_t *self, void *context);
    sm_state_t next_state;
} sm_state_config_t;

// State machine runtime handle (stored in RAM)
struct sm_handle_t
{
    sm_state_t cur_state;
    const sm_state_config_t *state_table;
    size_t num_states;
    void *context;
};

sm_state_t sm_run(sm_handle_t *self);

#endif // STATE_MACHINE_H
