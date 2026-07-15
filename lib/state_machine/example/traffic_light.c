#include "traffic_light.h"

sm_state_t green(sm_handle_t *smh, void *context)
{
    (void)smh;
    (void)context;
    return SM_GREEN;
}

sm_state_t yellow(sm_handle_t *smh, void *context)
{
    (void)smh;
    (void)context;
    return SM_YELLOW;
}

sm_state_t red(sm_handle_t *smh, void *context)
{
    (void)smh;
    (void)context;
    return SM_RED;
}
