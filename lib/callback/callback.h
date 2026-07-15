/*H************************************************************
 *
 * Callback (Observer)
 *
 *  Provides a lightweight, zero-heap event notification system.
 *  Observers are linked intrusively, allowing static or stack-based
 *  allocations without dynamic memory fragmentation.
 *
 *  Usage Example:
 *   1. Declare a subject: cb_subject_t my_subject;
 *      cb_subject_init(&my_subject);
 *
 *   2. Define a callback: void my_cb(void *ctx, const void *data);
 *
 *   3. Declare/initialize an observer:
 *      cb_observer_t my_obs = { &my_cb, &my_context, NULL };
 *
 *   4. Register the observer:
 *      cb_subject_register(&my_subject, &my_obs);
 *
 *   5. Trigger notification:
 *      cb_subject_notify(&my_subject, &event_data);
 *
 * @version 1.0.0
 *
 *************************************************************H*/

#ifndef CALLBACK_H
#define CALLBACK_H

#include <stdbool.h>

// Forward declarations
typedef struct cb_subject_t cb_subject_t;
typedef struct cb_observer_t cb_observer_t;

// Callback signature: accepts a user context pointer and a read-only event data pointer
typedef void (*cb_fn_t)(void *context, const void *event_data);

/**
 * @brief Observer structure (Subscriber)
 * 
 * Represents a registered listener. This structure is allocated by the client
 * (statically, globally, or on the stack). It holds a pointer to the callback 
 * function, a user-specific context, and a pointer to the next observer to 
 * form an intrusive singly-linked list.
 */
struct cb_observer_t
{
    cb_fn_t          callback;  // Notification handler function called on notify
    void            *context;   // User context pointer passed back to callback
    cb_observer_t   *next;      // Intrusive linked-list pointer to next observer
};

/**
 * @brief Subject structure (Publisher/Event Source)
 * 
 * Represents the publisher of events. It contains a single head pointer
 * to the first observer in the registered list. RAM footprint is minimal.
 */
struct cb_subject_t
{
    cb_observer_t   *head;      // Head of the registered observers list
};

// API Prototypes
void cb_subject_init(cb_subject_t *subject);
bool cb_subject_register(cb_subject_t *subject, cb_observer_t *observer);
bool cb_subject_unregister(cb_subject_t *subject, cb_observer_t *observer);
void cb_subject_notify(cb_subject_t *subject, const void *event_data);

#endif // CALLBACK_H
