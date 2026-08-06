/*H************************************************************
 *
 * Callback Pattern - Queue-Deferred Callback (ISR-safe)
 *
 *  Provides an ISR-safe callback dispatch mechanism.
 *  Instead of executing long-running or non-deterministic callback
 *  routines directly inside an Interrupt Service Routine (ISR), the
 *  interrupt handler posts the callback pointer and its argument context
 *  to a lock-free queue.
 *
 *  A lower-priority background task or main-loop polling routine then
 *  dequeues and executes the callbacks in thread context.
 *
 *  This implementation wraps the byte-based 'ring_buffer' library,
 *  serializing the callback execution structures into the buffer.
 *
 * @author John E Maddox
 *
 * @version 1.0.0
 *
 *************************************************************H*/

#ifndef CALLBACK_QUEUE_H
#define CALLBACK_QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "ring_buffer.h"

// Deferred callback function signature
typedef void (*cb_deferred_func_t)(void *context, uint32_t payload);

// Queue event container
typedef struct {
    cb_deferred_func_t func;
    void *context;
    uint32_t payload;
} cb_deferred_event_t;

// Queue handle structure, wraps the ring_buffer
typedef struct {
    rb_handle_t rb;
} cq_handle_t;

// API to initialize queue (heap-less, requires pre-allocated byte buffer of power-of-two size)
bool cq_init(cq_handle_t *self, uint8_t *byte_buf, size_t buf_size);

// API to post callback into the queue (ISR-safe, lock-free SPSC)
bool cq_post(cq_handle_t *self, cb_deferred_func_t func, void *context, uint32_t payload);

// API to dispatch next callback in the queue (runs in main loop or RTOS thread context)
bool cq_dispatch_one(cq_handle_t *self);

// API to check if queue is empty
bool cq_is_empty(const cq_handle_t *self);

#endif // CALLBACK_QUEUE_H
