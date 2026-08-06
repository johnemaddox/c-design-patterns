#include "sensor_sim.h"
#include <stddef.h>

// Private simulation context (RAM)
typedef struct {
    float sim_value;
    uint32_t sim_events;
    uint8_t enabled;
} sensor_sim_context_t;

static sensor_sim_context_t sim_context_data = {
    .sim_value = 100.0f,
    .sim_events = 0,
    .enabled = 0
};

// Static callbacks
static sensor_state_t sim_init(intf_handle_t *self)
{
    sensor_sim_context_t *ctx = (sensor_sim_context_t *)self->context;
    ctx->sim_events++;

    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_INIT;
    rtn.sens_state.value = (float)ctx->sim_events;
    return rtn;
}

static sensor_state_t sim_read(intf_handle_t *self, float *value)
{
    sensor_sim_context_t *ctx = (sensor_sim_context_t *)self->context;
    sensor_state_t rtn;
    
    if (!ctx->enabled)
    {
        rtn.intf_flag = INTF_ERR;
        rtn.sens_state.flag = SENS_READ;
        rtn.sens_state.value = 0.0f;
        return rtn;
    }
    
    ctx->sim_events++;
    if (value != NULL)
    {
        *value = ctx->sim_value;
    }

    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_READ;
    rtn.sens_state.value = ctx->sim_value;
    return rtn;
}

static sensor_state_t sim_calibrate(intf_handle_t *self)
{
    sensor_sim_context_t *ctx = (sensor_sim_context_t *)self->context;
    ctx->sim_events++;
    ctx->sim_value = 50.0f;

    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_CALIBRATE;
    rtn.sens_state.value = ctx->sim_value;
    return rtn;
}

static sensor_state_t sim_enable(intf_handle_t *self)
{
    sensor_sim_context_t *ctx = (sensor_sim_context_t *)self->context;
    ctx->sim_events++;
    ctx->enabled = 1;

    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_ENABLE;
    rtn.sens_state.value = (float)ctx->sim_events;
    return rtn;
}

static sensor_state_t sim_disable(intf_handle_t *self)
{
    sensor_sim_context_t *ctx = (sensor_sim_context_t *)self->context;
    ctx->sim_events++;
    ctx->enabled = 0;

    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_DISABLE;
    rtn.sens_state.value = (float)ctx->sim_events;
    return rtn;
}

// Ops Table (Flash / ROM)
static const sensor_ops_t sens_sim_ops = {
    .init      = &sim_init,
    .read      = &sim_read,
    .calibrate = &sim_calibrate,
    .enable    = &sim_enable,
    .disable   = &sim_disable
};

intf_flag_t sensor_sim_create(intf_handle_t *self)
{
    if (self == NULL)
    {
        return INTF_ERR;
    }
    self->ops = &sens_sim_ops;
    self->context = &sim_context_data;
    return INTF_OK;
}
