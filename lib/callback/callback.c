#include "callback.h"
#include <stddef.h>

void cb_subject_init(cb_subject_t *subject)
{
    if (subject != NULL)
    {
        subject->head = NULL;
    }
}

bool cb_subject_register(cb_subject_t *subject, cb_observer_t *observer)
{
    if (subject == NULL || observer == NULL || observer->callback == NULL)
    {
        return false;
    }

    // Prevent duplicate registration
    cb_observer_t *curr = subject->head;
    while (curr != NULL)
    {
        if (curr == observer)
        {
            return false;
        }
        curr = curr->next;
    }

    // Prepend to the intrusive list (O(1) insertion)
    observer->next = subject->head;
    subject->head = observer;

    return true;
}

bool cb_subject_unregister(cb_subject_t *subject, cb_observer_t *observer)
{
    if (subject == NULL || observer == NULL)
    {
        return false;
    }

    cb_observer_t *curr = subject->head;
    cb_observer_t *prev = NULL;

    while (curr != NULL)
    {
        if (curr == observer)
        {
            if (prev == NULL)
            {
                subject->head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            curr->next = NULL; // Clear links
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false; // Not registered
}

void cb_subject_notify(cb_subject_t *subject, const void *event_data)
{
    if (subject == NULL)
    {
        return;
    }

    cb_observer_t *curr = subject->head;
    while (curr != NULL)
    {
        if (curr->callback != NULL)
        {
            curr->callback(curr->context, event_data);
        }
        curr = curr->next;
    }
}
