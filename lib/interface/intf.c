#include "intf.h"
#include <stddef.h>

intf_state_t intf_init(intf_t *self)
{
    if (self == NULL || self->ops == NULL || self->ops->init == NULL)
    {
        intf_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return self->ops->init(self);
}

intf_state_t intf_capture(intf_t *self, uint32_t num_of_captures)
{
    if (self == NULL || self->ops == NULL || self->ops->capture == NULL)
    {
        intf_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return self->ops->capture(self, num_of_captures);
}

intf_state_t intf_start(intf_t *self)
{
    if (self == NULL || self->ops == NULL || self->ops->start == NULL)
    {
        intf_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return self->ops->start(self);
}

intf_state_t intf_stop(intf_t *self)
{
    if (self == NULL || self->ops == NULL || self->ops->stop == NULL)
    {
        intf_state_t err_state = { .intf_flag = INTF_ERR };
        return err_state;
    }
    return self->ops->stop(self);
}
