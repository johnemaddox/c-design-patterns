/*H************************************************************
 *
 * FiFo Ring Buffer
 *
 *  Performs basic FiFo operations for use in embedded systems.
 *  Maintains thread safe operations by using an empty buffer
 *  cell as a way to to prevent simultneous read and writes to
 *  the same buffer location. Each cell utilizes one byte.
 *
 *  Length is required to be a power of two, if it is not memory
 *  will not be allocated and RB_LEN_ERR will be returned. This
 *  will result in seg fault if the wrong size buffer is created
 *  and the flag is not checked.
 *
 * @author John E Maddox
 *
 * @version 1.0.0
 *
*************************************************************H*/

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    RB_OK,
    RB_LEN_ERR,
    RB_MEM_ERR,
    RB_EMPTY,
    RB_FULL
} rb_status_t;

typedef enum
{
    RB_TYPE_STOP_ON_FULL,
    RB_TYPE_OVERWRITE
} rb_type_t;

typedef struct
{
    uint8_t  *data;
    size_t    max_len;
    size_t    head_idx;
    size_t    tail_idx;
    rb_type_t type;
} rb_handle_t;

/*  - allocate buffer memory to rb_handle_t **rb
 *  - max length must be a power of 2 or RB_LEN_ERR is returned
 *  - one space is reserved, actual usable length is max_len-1
 */
rb_status_t rb_init(rb_handle_t **rb, size_t max_len, rb_type_t type);

rb_status_t rb_put(rb_handle_t *rb, uint8_t data_in);

rb_status_t rb_get(rb_handle_t *rb, uint8_t *data_out);

// resets buffer indexes to zero
void rb_reset(rb_handle_t *rb);

void rb_free(rb_handle_t** rb);

#endif // RING_BUFFER_H
