#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "state_machine.h"

typedef enum
{
    TL_GREEN,
    TL_YELLOW,
    TL_RED,
    TL_STATES_LEN
} tl_states_t;

/* To demonstrate, the context struct holds a persistent 
 * count while the test cycles through each state.
*/
typedef struct
{
    int green_calls;
    int yellow_calls;
    int red_calls;
} tl_context_t;

sm_state_t tl_green(sm_handle_t *self, void *context);
sm_state_t tl_yellow(sm_handle_t *self, void *context);
sm_state_t tl_red(sm_handle_t *self, void *context);

#endif // TRAFFIC_LIGHT_H
