#include "internals/event_internals/linked_widget.h"
#include "iterators/ei_iterator.h"
#include "helpers/ei_memory.h"

LIST_ITERATOR(ei_linked_widget_t, linked_widget)


// create linked widget from a widget ptr.

ei_linked_widget_t *ei_linked_widget_create(ei_widget_t *widget)
{
    if (widget == NULL)
        return NULL;
    ei_linked_widget_t *new = verified_calloc(1, sizeof(ei_linked_widget_t));
    new->widget = widget;
    new->linked_callbacks = NULL;
    new->next = NULL;
    return new;
}

// insert a callback in the linked callback list of this linked widget.
void ei_linked_widget_register_callback(ei_linked_widget_t *linked_widget, ei_callback_t callback, void *user_params)
{
    ei_linked_callback_t *new = ei_linked_callback_create(callback, user_params);
    ei_linked_callback_append(&linked_widget->linked_callbacks, new);
}

// remove a callback in the linked callback list of this linked widget.
void ei_linked_widget_unregister_callback(ei_linked_widget_t *linked_widget, ei_callback_t callback, void *user_params)
{
    ei_linked_callback_remove(&linked_widget->linked_callbacks, callback, user_params);
}

// append a widget in linked list at head
// If widget already in the list, returns it
// Otherwise returns the newly inserted widget
// Return NULL if failure

ei_linked_widget_t *ei_linked_widget_append(ei_linked_widget_t **linked_widget, ei_linked_widget_t *to_append)
{
    if (to_append == NULL)
        return NULL;
    ei_iterator_t linked_it = ei_it_begin(linked_widget, *linked_widget);
    for (; ei_it_valid(&linked_it); ei_it_next(&linked_it))
    {
        ei_linked_widget_t *current = ei_it_get_t(linked_widget, &linked_it);
        if (current->widget->pick_id == to_append->widget->pick_id)
        {
            ei_it_destroy(&linked_it);
            return current;
        }
    }
    ei_it_destroy(&linked_it);
    to_append->next = *linked_widget;
    *linked_widget = to_append;
    return to_append;
}


// Find a matching widget in the linked list. Return NULL if not found
ei_linked_widget_t *ei_linked_widget_find(ei_linked_widget_t *entry_point, ei_widget_t *widget)
{
    if (widget == NULL)
        return NULL;
    uint32_t pick_id = widget->pick_id;
    ei_iterator_t linked_it = ei_it_begin(linked_widget, entry_point);
    for (; ei_it_valid(&linked_it); ei_it_next(&linked_it))
    {
      ei_linked_widget_t *result = ei_it_get_t(linked_widget, &linked_it);
      if (result->widget->pick_id == pick_id)
      {
          ei_it_destroy(&linked_it);
          return result;
      }
    }
    ei_it_destroy(&linked_it);
    return NULL;
}

// delete a specific widget from the linked list. Only the first widget matching the pick_id is removed and freed
// Return the True if the deleted item if found. False otherwise
ei_bool_t ei_linked_widget_remove(ei_linked_widget_t **head, ei_widget_t *widget)
{
    if (widget == NULL)
        return EI_FALSE;
    ei_linked_widget_t sent;
    sent.next = *head;
    uint32_t pick_id = widget->pick_id;
    ei_linked_widget_t *prev = &sent;
    while(prev->next != NULL)
    {
        if (prev->next->widget->pick_id == pick_id)
        {
            // deletion
            ei_linked_widget_t *next = prev->next->next;
            ei_linked_callback_destroy(prev->next->linked_callbacks);
            free(prev->next);
            prev->next = next;
            *head = sent.next;
            return EI_TRUE;
        }
        prev = prev->next;
    }
    return EI_FALSE;
}

// free the entire linked list

void ei_linked_widget_destroy(ei_linked_widget_t *linked_widget)
{
    for (ei_linked_widget_t *it = linked_widget; it != NULL;)
    {
        ei_linked_widget_t *tmp = it->next;
        ei_linked_callback_destroy(it->linked_callbacks);
        free(it);
        it = tmp;
    }
}
