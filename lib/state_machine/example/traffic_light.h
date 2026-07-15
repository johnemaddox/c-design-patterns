#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "state_machine.h"

typedef enum
{
    SM_GREEN,
    SM_YELLOW,
    SM_RED,
    SM_STATES_LEN
} sm_states_t;

sm_state_t green(sm_handle_t *smh, void *context);
sm_state_t yellow(sm_handle_t *smh, void *context);
sm_state_t red(sm_handle_t *smh, void *context);

#endif // TRAFFIC_LIGHT_H
