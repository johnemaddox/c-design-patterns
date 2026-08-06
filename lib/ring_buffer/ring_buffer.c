#include "ring_buffer.h"
#include <stdbool.h>

static inline bool is_power_of_two(size_t num)
{
    return (num > 0) && ((num & (num - 1)) == 0);
}

// Optimization: bitwise mask wrapper for power-of-two lengths
static inline size_t update_idx(size_t idx, size_t max_len)
{
    return (idx + 1) & (max_len - 1);
}

rb_status_t rb_init(rb_handle_t *self, uint8_t *data_buf, size_t max_len, rb_type_t type)
{
    if (self == NULL || data_buf == NULL)
    {
        return RB_NULLPTR_ERR;
    }

    if (!is_power_of_two(max_len))
    {
        return RB_LEN_ERR;
    }

    self->data = data_buf;
    self->max_len = max_len;
    self->type = type;
    
    rb_reset(self);

    return RB_OK;
}

rb_status_t rb_put(rb_handle_t *self, uint8_t data_in)
{
    if (self == NULL || self->data == NULL)
    {
        return RB_NULLPTR_ERR;
    }

    size_t next_idx = update_idx(self->head_idx, self->max_len);

    if (next_idx == self->tail_idx)
    {
        if (self->type == RB_TYPE_OVERWRITE)
        {
            self->tail_idx = update_idx(self->tail_idx, self->max_len);
        }
        else
        {
            return RB_FULL;
        }
    }

    self->data[self->head_idx] = data_in;
    self->head_idx = next_idx;

    return RB_OK;
}

rb_status_t rb_get(rb_handle_t *self, uint8_t *data_out)
{
    if (self == NULL || self->data == NULL || data_out == NULL)
    {
        return RB_NULLPTR_ERR;
    }

    if (self->head_idx == self->tail_idx)
    {
        return RB_EMPTY;
    }

    *data_out = self->data[self->tail_idx];
    self->tail_idx = update_idx(self->tail_idx, self->max_len);

    return RB_OK;
}

size_t rb_occupied(const rb_handle_t *self)
{
    if (self == NULL || self->data == NULL)
    {
        return 0;
    }
    return (self->head_idx - self->tail_idx) & (self->max_len - 1);
}

size_t rb_available(const rb_handle_t *self)
{
    if (self == NULL || self->data == NULL)
    {
        return 0;
    }
    return (self->max_len - 1) - rb_occupied(self);
}

void rb_reset(rb_handle_t *self)
{
    if (self != NULL)
    {
        self->head_idx = 0;
        self->tail_idx = 0;
    }
}
