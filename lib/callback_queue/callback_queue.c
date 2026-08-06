#include "callback_queue.h"
#include "ring_buffer.h"

bool cq_init(cq_handle_t *self, uint8_t *byte_buf, size_t buf_size) {
    if (self == NULL || byte_buf == NULL) {
        return false;
    }
    // Initialize the underlying byte ring buffer
    rb_status_t status = rb_init(&self->rb, byte_buf, buf_size, RB_TYPE_STOP_ON_FULL);
    return (status == RB_OK);
}

bool cq_post(cq_handle_t *self, cb_deferred_func_t func, void *context, uint32_t payload) {
    if (self == NULL || func == NULL) {
        return false;
    }

    size_t free_space = rb_available(&self->rb);

    // Check if we have enough space to serialize a cb_deferred_event_t struct
    if (free_space < sizeof(cb_deferred_event_t)) {
        return false;
    }

    // Serialize individual fields sequentially, avoiding struct padding issues
    // 1. Function pointer
    uint8_t *p_func = (uint8_t *)&func;
    for (size_t i = 0; i < sizeof(func); i++) {
        rb_put(&self->rb, p_func[i]);
    }
    // 2. Context pointer
    uint8_t *p_ctx = (uint8_t *)&context;
    for (size_t i = 0; i < sizeof(context); i++) {
        rb_put(&self->rb, p_ctx[i]);
    }
    // 3. Payload
    uint8_t *p_payload = (uint8_t *)&payload;
    for (size_t i = 0; i < sizeof(payload); i++) {
        rb_put(&self->rb, p_payload[i]);
    }

    // 4. Explicit zero-padding to align to structure size
    size_t written = sizeof(func) + sizeof(context) + sizeof(payload);
    size_t padding_bytes = sizeof(cb_deferred_event_t) - written;
    for (size_t i = 0; i < padding_bytes; i++) {
        rb_put(&self->rb, 0);
    }

    return true;
}

bool cq_dispatch_one(cq_handle_t *self) {
    if (self == NULL) {
        return false;
    }

    size_t occupied = rb_occupied(&self->rb);

    // Only read if a full struct has been queued
    if (occupied < sizeof(cb_deferred_event_t)) {
        return false;
    }

    cb_deferred_event_t event;
    uint8_t *ptr = (uint8_t *)&event;
    for (size_t i = 0; i < sizeof(cb_deferred_event_t); i++) {
        rb_get(&self->rb, &ptr[i]);
    }

    if (event.func != NULL) {
        event.func(event.context, event.payload);
    }

    return true;
}

bool cq_is_empty(const cq_handle_t *self) {
    if (self == NULL) {
        return true;
    }

    size_t occupied = rb_occupied(&self->rb);
    return (occupied < sizeof(cb_deferred_event_t));
}
