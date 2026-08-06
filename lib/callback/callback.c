#include "callback.h"

void cb_register(cb_handle_t *self, cb_func_t func, void *context) {
    if (self != NULL) {
        self->func = func;
        self->context = context;
    }
}

void cb_fire(const cb_handle_t *self, uint32_t event_data) {
    if (self != NULL && self->func != NULL) {
        self->func(self->context, event_data);
    }
}
