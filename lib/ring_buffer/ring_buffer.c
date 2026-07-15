/*H************************************************************
 *
 * FiFo Ring Buffer
 *
 * @author John E Maddox
 *
*************************************************************H*/

#include "ring_buffer.h"
#include <stdbool.h>

static inline bool is_power_of_two(size_t num)
{
    return (num > 0) && ((num & (num - 1)) == 0);
}

rb_status_t rb_init(rb_handle_t *rb, uint8_t *data_buf, size_t max_len, rb_type_t type)
{
    if (rb == NULL || data_buf == NULL)
    {
        return RB_PVAL_ERR;
    }

    if (!is_power_of_two(max_len))
    {
        return RB_LEN_ERR;
    }

    rb->data = data_buf;
    rb->max_len = max_len;
    rb->type = type;
    
    rb_reset(rb);

    return RB_OK;
}

rb_status_t rb_put(rb_handle_t *rb, uint8_t data_in)
{
    if (rb == NULL || rb->data == NULL)
    {
        return RB_PVAL_ERR;
    }

    // Optimization: bitwise mask wrapper for power-of-two lengths
    size_t next_idx = (rb->head_idx + 1) & (rb->max_len - 1);

    if (next_idx == rb->tail_idx)
    {
        if (rb->type == RB_TYPE_OVERWRITE)
        {
            rb->tail_idx = (rb->tail_idx + 1) & (rb->max_len - 1);
        }
        else
        {
            return RB_FULL;
        }
    }

    rb->data[rb->head_idx] = data_in;
    rb->head_idx = next_idx;

    return RB_OK;
}

rb_status_t rb_get(rb_handle_t *rb, uint8_t *data_out)
{
    if (rb == NULL || rb->data == NULL || data_out == NULL)
    {
        return RB_PVAL_ERR;
    }

    if (rb->head_idx == rb->tail_idx)
    {
        return RB_EMPTY;
    }

    *data_out = rb->data[rb->tail_idx];
    rb->tail_idx = (rb->tail_idx + 1) & (rb->max_len - 1);

    return RB_OK;
}

void rb_reset(rb_handle_t *rb)
{
    if (rb != NULL)
    {
        rb->head_idx = 0;
        rb->tail_idx = 0;
    }
}
