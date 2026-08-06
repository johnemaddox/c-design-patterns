#include "state_machine.h"

sm_state_t sm_run(sm_handle_t *self)
{
    if (self == NULL || self->state_table == NULL || self->cur_state >= self->num_states)
    {
        return 0;
    }

    const sm_state_config_t *state = &self->state_table[self->cur_state];
    sm_state_t rtn = self->cur_state;

    if (state->event != NULL)
    {
        rtn = state->event(self, self->context);
    }

    self->cur_state = state->next_state;
    return rtn;
}
