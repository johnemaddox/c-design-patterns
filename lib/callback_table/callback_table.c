#include "callback_table.h"

void ct_init(ct_handle_t *self, const ct_table_t *ops, void *context) {
    if (self != NULL) {
        self->ops = ops;
        self->context = context;
    }
}

void ct_fire_started(const ct_handle_t *self) {
    if (self != NULL && self->ops != NULL && self->ops->on_started != NULL) {
        self->ops->on_started(self->context);
    }
}

void ct_fire_data(const ct_handle_t *self, const uint8_t *buffer, size_t size) {
    if (self != NULL && self->ops != NULL && self->ops->on_data != NULL) {
        self->ops->on_data(self->context, buffer, size);
    }
}

void ct_fire_error(const ct_handle_t *self, int32_t error_code) {
    if (self != NULL && self->ops != NULL && self->ops->on_error != NULL) {
        self->ops->on_error(self->context, error_code);
    }
}
