#include "state_machine.h"

sm_states_t sm_run(sm_handle_t *smh)
{
    sm_states_t rtn = smh->states[smh->cur_state].event();
    smh->cur_state = smh->states[smh->cur_state].next_state;
    return rtn;
};
