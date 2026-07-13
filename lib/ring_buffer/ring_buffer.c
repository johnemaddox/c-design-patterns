/*H************************************************************
 *
 * FiFo Ring Buffer
 *
 * @author John E Maddox
 *
*************************************************************H*/

#include "ring_buffer.h"
#include <stdbool.h>
#include <stdlib.h>

static bool is_power_of_two(size_t num)
{
    if ( (num == 0) ||
         ((num & (num - 1)) != 0) )
    { 
        return false;
    }
    return true;
}

static size_t get_next_idx(size_t idx, size_t max)
{
    return (idx + 1) % max;
}

rb_status_t rb_init(rb_handle_t **rb, size_t max_len, rb_type_t type)
{
    if (!is_power_of_two(max_len)) { return RB_LEN_ERR; }

    (*rb) = (rb_handle_t*)malloc(sizeof(rb_handle_t));

    if ((*rb) == NULL) { return RB_MEM_ERR; }

    (*rb)->data = (uint8_t*)calloc(max_len, sizeof(uint8_t));

    if ((*rb)->data == NULL)
    { 
        free(*rb);
        *rb = NULL;
        return RB_MEM_ERR;
    }

    (*rb)->max_len = max_len;
    (*rb)->type = type;
    rb_reset(*rb);

    return RB_OK;
}

rb_status_t rb_put(rb_handle_t *rb, uint8_t data_in)
{
    size_t next_idx = get_next_idx(rb->head_idx, rb->max_len);

    if (next_idx == rb->tail_idx)
    { 
        if (rb->type == RB_TYPE_OVERWRITE)
        {
            rb->tail_idx = get_next_idx(rb->tail_idx, rb->max_len);
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
    if (rb->head_idx == rb->tail_idx) { return RB_EMPTY; }

    *data_out = rb->data[rb->tail_idx];

    rb->tail_idx = get_next_idx(rb->tail_idx, rb->max_len);

    return RB_OK;
}

void rb_reset(rb_handle_t *rb)
{
    rb->head_idx = 0;
    rb->tail_idx = 0;
}

void rb_free(rb_handle_t** rb)
{
    free((*rb)->data);
    (*rb)->data = NULL;

    free(*rb);
    *rb = NULL;
}
