#include "sensor_intf.h"
#include <stddef.h>

sensor_state_t sensor_init(intf_handle_t *self)
{
    if (self == NULL || self->ops == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const sensor_ops_t *ops = (const sensor_ops_t *)self->ops;
    if (ops->init == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->init(self);
}

sensor_state_t sensor_read(intf_handle_t *self, float *value)
{
    if (self == NULL || self->ops == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const sensor_ops_t *ops = (const sensor_ops_t *)self->ops;
    if (ops->read == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->read(self, value);
}

sensor_state_t sensor_calibrate(intf_handle_t *self)
{
    if (self == NULL || self->ops == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const sensor_ops_t *ops = (const sensor_ops_t *)self->ops;
    if (ops->calibrate == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->calibrate(self);
}

sensor_state_t sensor_enable(intf_handle_t *self)
{
    if (self == NULL || self->ops == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const sensor_ops_t *ops = (const sensor_ops_t *)self->ops;
    if (ops->enable == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->enable(self);
}

sensor_state_t sensor_disable(intf_handle_t *self)
{
    if (self == NULL || self->ops == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const sensor_ops_t *ops = (const sensor_ops_t *)self->ops;
    if (ops->disable == NULL)
    {
        sensor_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->disable(self);
}
