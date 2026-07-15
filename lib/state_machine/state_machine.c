#include "state_machine.h"

sm_state_t sm_run(sm_handle_t *smh)
{
    if (smh == NULL || smh->state_table == NULL || smh->cur_state >= smh->num_states)
    {
        return 0;
    }

    const sm_state_config_t *state = &smh->state_table[smh->cur_state];
    sm_state_t rtn = smh->cur_state;

    if (state->event != NULL)
    {
        rtn = state->event(smh, smh->context);
    }

    smh->cur_state = state->next_state;
    return rtn;
}
