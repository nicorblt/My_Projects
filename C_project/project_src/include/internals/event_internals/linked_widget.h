#ifndef EVENT_INTERNAL_LINKED_WIDGET_H
#define EVENT_INTERNAL_LINKED_WIDGET_H

#include "ei_widget.h"
#include "internals/event_internals/linked_callback.h"
#include "ei_types.h"

struct ei_iterator_t;

typedef struct ei_linked_widget_t
{
    ei_widget_t *widget;
    ei_linked_callback_t *linked_callbacks;
    struct ei_linked_widget_t *next;
} ei_linked_widget_t;


// create linked widget from a widget ptr.
ei_linked_widget_t *ei_linked_widget_create(ei_widget_t *widget);

// insert a callback in the linked callback list of this linked widget as the new last callback to be called.
void ei_linked_widget_register_callback(ei_linked_widget_t *linked_widget, ei_callback_t callback, void *user_params);

// remove a callback in the linked callback list of this linked widget.
void ei_linked_widget_unregister_callback(ei_linked_widget_t *linked_widget, ei_callback_t callback, void *user_params);

// append a widget in linked list at head
// If widget already in the list, returns it
// Otherwise returns the newly inserted widget
ei_linked_widget_t *ei_linked_widget_append(ei_linked_widget_t **linked_widget, ei_linked_widget_t *to_append);

// Find a matching widget in the linked list. Return NULL if not found
ei_linked_widget_t *ei_linked_widget_find(ei_linked_widget_t *entry_point, ei_widget_t *widget);

// delete a specific widget from the linked list. Only the first widget matching the pick_id is removed and freed
ei_bool_t ei_linked_widget_remove(ei_linked_widget_t **head, ei_widget_t *widget);

// free the entire linked list
void ei_linked_widget_destroy(ei_linked_widget_t *linked_widget);


/* ***** ITERATORS ***** */
void *ei_it_linked_widget_next(struct ei_iterator_t *it);
ei_linked_widget_t *ei_it_linked_widget_get(struct ei_iterator_t *it);
struct ei_iterator_t ei_it_linked_widget_begin(ei_linked_widget_t *iterable);

#endif