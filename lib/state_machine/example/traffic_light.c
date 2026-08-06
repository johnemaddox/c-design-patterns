#include "traffic_light.h"

sm_state_t tl_green(sm_handle_t *self, void *context)
{
    (void)self;
    tl_context_t *ctx = (tl_context_t *)context;
    if (ctx != NULL)
    {
        ctx->green_calls++;
    }
    return TL_GREEN;
}

sm_state_t tl_yellow(sm_handle_t *handle, void *context)
{
    (void)handle;
    tl_context_t *ctx = (tl_context_t *)context;
    if (ctx != NULL)
    {
        ctx->yellow_calls++;
    }
    return TL_YELLOW;
}

sm_state_t tl_red(sm_handle_t *handle, void *context)
{
    (void)handle;
    tl_context_t *ctx = (tl_context_t *)context;
    if (ctx != NULL)
    {
        ctx->red_calls++;
    }
    return TL_RED;
}
