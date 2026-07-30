/*H************************************************************
 *
 * FiFo Ring Buffer
 *
 *  Performs basic FiFo operations for use in embedded systems.
 *  Maintains thread-safe operations by using an empty buffer
 *  cell as a way to prevent simultaneous read and writes to
 *  the same buffer location. Each cell utilizes one byte.
 *
 *  Length is required to be a power of two. If it is not, the
 *  initialization will fail and RB_LEN_ERR will be returned.
 *  This library is completely heap-less, relying on caller
 *  provided static or stack memory buffers.
 *
 *  Supports two behavior modes when the buffer is full:
 *   - STOP_ON_FULL: Rejects new writes and returns RB_FULL.
 *   - OVERWRITE: Overwrites the oldest stored byte and advances
 *     the tail index to accommodate new incoming data.
 *
 * @author John E Maddox
 *
 * @version 1.1.0
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
    RB_NULLPTR_ERR,
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

/*  - Initialize a pre-allocated ring buffer handle.
 *  - data_buf must point to an array of size max_len.
 *  - max_len must be a power of 2.
 */
rb_status_t rb_init(rb_handle_t *rb, uint8_t *data_buf, size_t max_len, rb_type_t type);

rb_status_t rb_put(rb_handle_t *rb, uint8_t data_in);

rb_status_t rb_get(rb_handle_t *rb, uint8_t *data_out);

// resets buffer indexes to zero
void rb_reset(rb_handle_t *rb);

#endif // RING_BUFFER_H
