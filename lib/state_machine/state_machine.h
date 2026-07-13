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

typedef enum
{
    SM_GREEN,
    SM_YELLOW,
    SM_RED,
    SM_STATES_LEN
} sm_states_t;

typedef struct
{
    sm_states_t cur_state;
    struct
    {
        sm_states_t (*event)();
        sm_states_t next_state;
    } states[SM_STATES_LEN];
} sm_handle_t;

sm_states_t sm_run(sm_handle_t *smh);

#endif // STATE_MACHINE_H
