/*H************************************************************
 *
 * Observer Pattern
 *
 *  Provides a lightweight, zero-heap event notification system.
 *  Observers are linked intrusively, allowing static or stack-based
 *  allocations without dynamic memory fragmentation.
 *
 *  Usage Example:
 *   1. Declare a subject: obs_subject_t my_subject;
 *      obs_subject_init(&my_subject);
 *
 *   2. Define a callback: void my_cb(void *ctx, const void *data);
 *
 *   3. Declare/initialize an observer:
 *      obs_observer_t my_obs = { &my_cb, &my_context, NULL };
 *
 *   4. Register the observer:
 *      obs_subject_register(&my_subject, &my_obs);
 *
 *   5. Trigger notification:
 *      obs_subject_notify(&my_subject, &event_data);
 *
 * @version 2.0.0
 *
 *************************************************************H*/

#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdbool.h>

// Forward declarations
typedef struct obs_subject_t obs_subject_t;
typedef struct obs_observer_t obs_observer_t;

// Callback signature: accepts a user context pointer and a read-only event data pointer
typedef void (*obs_fn_t)(void *context, const void *event_data);

/**
 * @brief Observer structure (Subscriber)
 * 
 * Represents a registered listener. This structure is allocated by the client
 * (statically, globally, or on the stack). It holds a pointer to the callback 
 * function, a user-specific context, and a pointer to the next observer to 
 * form an intrusive singly-linked list.
 */
struct obs_observer_t
{
    obs_fn_t          callback;  // Notification handler function called on notify
    void            *context;   // User context pointer passed back to callback
    obs_observer_t   *next;      // Intrusive linked-list pointer to next observer
};

/**
 * @brief Subject structure (Publisher/Event Source)
 * 
 * Represents the publisher of events. It contains a single head pointer
 * to the first observer in the registered list. RAM footprint is minimal.
 */
struct obs_subject_t
{
    obs_observer_t   *head;      // Head of the registered observers list
};

// API Prototypes
void obs_subject_init(obs_subject_t *subject);
bool obs_subject_register(obs_subject_t *subject, obs_observer_t *observer);
bool obs_subject_unregister(obs_subject_t *subject, obs_observer_t *observer);
void obs_subject_notify(obs_subject_t *subject, const void *event_data);

#endif // OBSERVER_H
