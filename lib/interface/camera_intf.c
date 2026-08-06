#include "camera_intf.h"
#include <stddef.h>

camera_state_t camera_init(intf_handle_t *self)
{
    if (self == NULL || self->ops == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const camera_ops_t *ops = (const camera_ops_t *)self->ops;
    if (ops->init == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->init(self);
}

camera_state_t camera_capture(intf_handle_t *self, uint32_t num_of_captures)
{
    if (self == NULL || self->ops == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const camera_ops_t *ops = (const camera_ops_t *)self->ops;
    if (ops->capture == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->capture(self, num_of_captures);
}

camera_state_t camera_start(intf_handle_t *self)
{
    if (self == NULL || self->ops == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const camera_ops_t *ops = (const camera_ops_t *)self->ops;
    if (ops->start == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->start(self);
}

camera_state_t camera_stop(intf_handle_t *self)
{
    if (self == NULL || self->ops == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    const camera_ops_t *ops = (const camera_ops_t *)self->ops;
    if (ops->stop == NULL)
    {
        camera_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return ops->stop(self);
}
