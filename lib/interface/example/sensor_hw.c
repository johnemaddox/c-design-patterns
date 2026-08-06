#include "sensor_hw.h"
#include <stddef.h>

// Private hardware context (RAM)
typedef struct {
    float last_value;
    uint32_t read_count;
    uint8_t enabled;
} sensor_hw_context_t;

// Local static context instance
static sensor_hw_context_t hw_context_data = {
    .last_value = 25.0f,
    .read_count = 0,
    .enabled = 0
};

// Static callbacks implementing driver operations
static sensor_state_t hw_init(intf_handle_t *self)
{
    (void)self;
    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_INIT;
    rtn.sens_state.value = 0.0f;
    return rtn;
}

static sensor_state_t hw_read(intf_handle_t *self, float *value)
{
    sensor_hw_context_t *ctx = (sensor_hw_context_t *)self->context;
    sensor_state_t rtn;
    
    if (!ctx->enabled)
    {
        rtn.intf_flag = INTF_ERR;
        rtn.sens_state.flag = SENS_READ;
        rtn.sens_state.value = 0.0f;
        return rtn;
    }
    
    ctx->read_count++;
    ctx->last_value += 0.5f;
    if (value != NULL)
    {
        *value = ctx->last_value;
    }
    
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_READ;
    rtn.sens_state.value = ctx->last_value;
    return rtn;
}

static sensor_state_t hw_calibrate(intf_handle_t *self)
{
    sensor_hw_context_t *ctx = (sensor_hw_context_t *)self->context;
    ctx->last_value = 25.0f;

    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_CALIBRATE;
    rtn.sens_state.value = ctx->last_value;
    return rtn;
}

static sensor_state_t hw_enable(intf_handle_t *self)
{
    sensor_hw_context_t *ctx = (sensor_hw_context_t *)self->context;
    ctx->enabled = 1;

    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_ENABLE;
    rtn.sens_state.value = 1.0f;
    return rtn;
}

static sensor_state_t hw_disable(intf_handle_t *self)
{
    sensor_hw_context_t *ctx = (sensor_hw_context_t *)self->context;
    ctx->enabled = 0;

    sensor_state_t rtn;
    rtn.intf_flag = INTF_OK;
    rtn.sens_state.flag = SENS_DISABLE;
    rtn.sens_state.value = 0.0f;
    return rtn;
}

// Ops Table (Flash / ROM)
static const sensor_ops_t sens_hw_ops = {
    .init      = &hw_init,
    .read      = &hw_read,
    .calibrate = &hw_calibrate,
    .enable    = &hw_enable,
    .disable   = &hw_disable
};

intf_flag_t sensor_hw_create(intf_handle_t *self)
{
    if (self == NULL)
    {
        return INTF_ERR;
    }
    self->ops = &sens_hw_ops;
    self->context = &hw_context_data;
    return INTF_OK;
}
