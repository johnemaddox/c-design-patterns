#include "camera_sim.h"
#include <stddef.h>

// Private simulation context (RAM)
typedef struct {
    uint32_t capture_count;
    uint32_t sim_events;
} camera_sim_context_t;

static camera_sim_context_t sim_context_data = {
    .capture_count = 0,
    .sim_events = 0
};

// Static callbacks
static intf_state_t sim_init(intf_t *self)
{
    camera_sim_context_t *ctx = (camera_sim_context_t *)self->context;
    ctx->sim_events++;

    intf_state_t rtn;
    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_INIT;
    rtn.cam_state.extra = ctx->sim_events;
    return rtn;
}

static intf_state_t sim_capture(intf_t *self, uint32_t num_of_captures)
{
    camera_sim_context_t *ctx = (camera_sim_context_t *)self->context;
    ctx->capture_count += num_of_captures;
    ctx->sim_events++;

    intf_state_t rtn;
    rtn.intf_flag       = INTF_OK;
    rtn.cam_state.flag  = CAM_CAPTURE;
    rtn.cam_state.extra = num_of_captures;
    return rtn;
}

static intf_state_t sim_start(intf_t *self)
{
    camera_sim_context_t *ctx = (camera_sim_context_t *)self->context;
    ctx->sim_events++;

    intf_state_t rtn;
    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_START;
    rtn.cam_state.extra = ctx->sim_events;
    return rtn;
}

static intf_state_t sim_stop(intf_t *self)
{
    camera_sim_context_t *ctx = (camera_sim_context_t *)self->context;
    ctx->sim_events++;

    intf_state_t rtn;
    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_STOP;
    rtn.cam_state.extra = ctx->sim_events;
    return rtn;
}

// Ops Table (Flash / ROM)
static const intf_ops_t cam_sim_ops = {
    .init    = &sim_init,
    .capture = &sim_capture,
    .start   = &sim_start,
    .stop    = &sim_stop
};

intf_flag_t camera_sim_create(intf_t *self)
{
    if (self == NULL)
    {
        return INTF_ERR;
    }
    self->ops = &cam_sim_ops;
    self->context = &sim_context_data;
    return INTF_OK;
}
