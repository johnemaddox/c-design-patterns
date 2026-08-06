/*H************************************************************
 *
 * Callback Pattern - Stateful Reentrant Callback
 *
 *  A standard callback pattern pairing a function pointer with a
 *  void *context pointer. This allows callbacks to be reentrant,
 *  instance-specific, and decoupled from global variables.
 *
 * @author John E Maddox
 *
 * @version 1.0.0
 *
 *************************************************************H*/

#ifndef CALLBACK_H
#define CALLBACK_H

#include <stddef.h>
#include <stdint.h>

// Callback type definition
typedef void (*cb_func_t)(void *context, uint32_t event_data);

// Callback handle structure (RAM-based)
typedef struct {
    cb_func_t func;
    void *context;
} cb_handle_t;

// API to register a callback
void cb_register(cb_handle_t *self, cb_func_t func, void *context);

// API to fire/execute the callback
void cb_fire(const cb_handle_t *self, uint32_t event_data);

#endif // CALLBACK_H
