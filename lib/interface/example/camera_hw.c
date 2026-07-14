#include "camera_hw.h"
#include <stddef.h>

// Private hardware context (RAM)
typedef struct {
    uint32_t capture_count;
} camera_hw_context_t;

// Local static context instance
static camera_hw_context_t hw_context_data = {
    .capture_count = 0
};

// Static callbacks implementing driver operations
static intf_state_t hw_init(intf_t *self)
{
    (void)self;
    intf_state_t rtn;
    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_INIT;
    rtn.cam_state.extra = 0;
    return rtn;
}

static intf_state_t hw_capture(intf_t *self, uint32_t num_of_captures)
{
    camera_hw_context_t *ctx = (camera_hw_context_t *)self->context;
    ctx->capture_count += num_of_captures;

    intf_state_t rtn;
    rtn.intf_flag       = INTF_OK;
    rtn.cam_state.flag  = CAM_CAPTURE;
    rtn.cam_state.extra = num_of_captures;
    return rtn;
}

static intf_state_t hw_start(intf_t *self)
{
    (void)self;
    intf_state_t rtn;
    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_START;
    rtn.cam_state.extra = 0;
    return rtn;
}

static intf_state_t hw_stop(intf_t *self)
{
    (void)self;
    intf_state_t rtn;
    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_STOP;
    rtn.cam_state.extra = 0;
    return rtn;
}

// Ops Table (Flash / ROM)
static const intf_ops_t cam_hw_ops = {
    .init    = &hw_init,
    .capture = &hw_capture,
    .start   = &hw_start,
    .stop    = &hw_stop
};

intf_flag_t camera_hw_create(intf_t *self)
{
    if (self == NULL)
    {
        return INTF_ERR;
    }
    self->ops = &cam_hw_ops;
    self->context = &hw_context_data;
    return INTF_OK;
}
