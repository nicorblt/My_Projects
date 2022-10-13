#include "internals/event_internals/linked_callback.h"
#include "helpers/ei_memory.h"
#include "iterators/ei_iterator.h"



// create empty linked list callback

ei_linked_callback_t *ei_linked_callback_create(ei_callback_t callback, void *user_params)
{
    if (callback == NULL)
        return NULL;
    ei_linked_callback_t *new = verified_calloc(1, sizeof(ei_linked_callback_t));
    new->user_params = user_params;
    new->next = NULL;
    new->tail = &new;
    new->callback = callback;
    return new;
}

// Insert 'to_append" as the next element of the tail, then change the tail to to_append;
// Return the 'linked_list' if success, otherwise return NULL

ei_linked_callback_t *ei_linked_callback_append(ei_linked_callback_t **head, ei_linked_callback_t *to_append)
{
    if (to_append == NULL)
        return NULL;
    if (*head == NULL)
    {
        *to_append->tail = to_append;
        to_append->next = NULL;
        *head = to_append;
        return *head;
    }
    // if head is non NULL, its tail is non NULL 
    to_append->tail = (*head)->tail;
    (*(*head)->tail)->next = to_append;
    *(*head)->tail = to_append;
    return *head;
}


// delete a specific (callback + user_data) from the linked list. Only the first match found is removed and freed
void ei_linked_callback_remove(ei_linked_callback_t **head, ei_callback_t callback, void* user_params)
{
    ei_linked_callback_t sent;
    sent.next = *head;
    ei_linked_callback_t *prev = &sent;
    while(prev->next != NULL)
    {
        if (prev->next->callback == callback && prev->next->user_params == user_params)
        {
            // deletion
            ei_linked_callback_t *next = prev->next->next;
            free(prev->next);
            prev->next = next;
            *head = sent.next;
            return;
        }
        prev = prev->next;
    }
}

// free the entire linked list

void ei_linked_callback_destroy(ei_linked_callback_t *first_link)
{
    for (ei_linked_callback_t *it = first_link; it != NULL; )
    {
        ei_linked_callback_t *tmp = it->next;
        free(it->user_params);
        free(it);
        it = tmp;
    }
}

// iterate throughout the linked list
LIST_ITERATOR(ei_linked_callback_t, linked_callback)