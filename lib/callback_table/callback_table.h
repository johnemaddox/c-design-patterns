/*H************************************************************
 *
 * Callback Pattern - Operations Table (VTable) Callback
 *
 *  Minimizes SRAM consumption when a driver/module registers multiple
 *  callbacks by storing the callback table in Flash (ROM) and
 *  referencing it via a single pointer in RAM.
 *
 * @author John E Maddox
 *
 * @version 1.0.0
 *
 *************************************************************H*/

#ifndef CALLBACK_TABLE_H
#define CALLBACK_TABLE_H

#include <stddef.h>
#include <stdint.h>

// Collection of related event callbacks (designed to reside in Flash/ROM)
typedef struct {
    void (*on_started)(void *context);
    void (*on_data)(void *context, const uint8_t *buffer, size_t size);
    void (*on_error)(void *context, int32_t error_code);
} ct_table_t;

// Module handle storing a pointer to the const ops table in Flash and the context in RAM
typedef struct {
    const ct_table_t *ops; // points to const static table in Flash
    void *context;         // instance context in RAM
} ct_handle_t;

// API to initialize the ops handle
void ct_init(ct_handle_t *self, const ct_table_t *ops, void *context);

// APIs to invoke the callbacks safely
void ct_fire_started(const ct_handle_t *self);
void ct_fire_data(const ct_handle_t *self, const uint8_t *buffer, size_t size);
void ct_fire_error(const ct_handle_t *self, int32_t error_code);

#endif // CALLBACK_TABLE_H
