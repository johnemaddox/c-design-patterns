/*H************************************************************
 *
 * Finite State Machine
 *
 *  The Finite State Machine is great for breaking down and
 *  managing complex tasks, such as a Stop Light System.
 *
 * @author John E Maddox
 *
 * @version 1.0.0
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
    sm_state_t (*event)(sm_handle_t *smh, void *context);
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

sm_state_t sm_run(sm_handle_t *smh);

#endif // STATE_MACHINE_H
