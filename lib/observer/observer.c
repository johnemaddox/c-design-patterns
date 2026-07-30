#include "observer.h"
#include <stddef.h>

void obs_subject_init(obs_subject_t *subject)
{
    if (subject != NULL)
    {
        subject->head = NULL;
    }
}

bool obs_subject_register(obs_subject_t *subject, obs_observer_t *observer)
{
    if (subject == NULL || observer == NULL || observer->callback == NULL)
    {
        return false;
    }

    // Prevent duplicate registration
    obs_observer_t *curr = subject->head;
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

bool obs_subject_unregister(obs_subject_t *subject, obs_observer_t *observer)
{
    if (subject == NULL || observer == NULL)
    {
        return false;
    }

    obs_observer_t *curr = subject->head;
    obs_observer_t *prev = NULL;

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

void obs_subject_notify(obs_subject_t *subject, const void *event_data)
{
    if (subject == NULL)
    {
        return;
    }

    obs_observer_t *curr = subject->head;
    while (curr != NULL)
    {
        if (curr->callback != NULL)
        {
            curr->callback(curr->context, event_data);
        }
        curr = curr->next;
    }
}
