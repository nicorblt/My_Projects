#ifndef EVENT_INTERNAL_LINKED_TAG_AND_WIDGETS_H
#define EVENT_INTERNAL_LINKED_TAG_AND_WIDGETS_H

#include "internals/event_internals/widget_hashmap.h"
#include "ei_event.h"

struct ei_iterator_t;

typedef struct ei_linked_tag_and_widgets_t
{
    ei_tag_t tag;
    ei_widget_hashmap_t *associated_widgets;
    ei_linked_callback_t *tag_callbacks;
    struct ei_linked_tag_and_widgets_t *next;
} ei_linked_tag_and_widgets_t;


// create linked widget_and_tag from a tag 
ei_linked_tag_and_widgets_t *ei_linked_tag_and_widgets_create(ei_tag_t tag);

// associate a widget with this tag.
void ei_linked_tag_and_widgets_register(ei_linked_tag_and_widgets_t *link, ei_widget_t *widget, ei_callback_t callback, void *user_params);
// Register the given callback to every widgets alraedy registered in this linked_tag
void ei_linked_tag_and_widgets_register_tag_callback(ei_linked_tag_and_widgets_t *link, ei_callback_t callback, void *user_params);
// Register every widget of the application as managed by this list
void ei_linked_tag_and_widgets_register_all_widgets(ei_linked_tag_and_widgets_t *link, ei_callback_t callback, void *user_params);


void ei_linked_tag_and_widgets_unregister_widget(ei_linked_tag_and_widgets_t *link, ei_widget_t *widget);
void ei_linked_tag_and_widgets_unregister_callback(ei_linked_tag_and_widgets_t *link, ei_widget_t *widget, ei_callback_t callback, void *user_params);
void ei_linked_tag_and_widgets_unregister_tag_callback(ei_linked_tag_and_widgets_t *link, ei_callback_t callback, void *user_params);

ei_linked_callback_t *ei_linked_tag_and_widgets_get_tag_callbacks(ei_linked_tag_and_widgets_t *link);
ei_linked_callback_t *ei_linked_tag_and_widgets_get_widget_callbacks(ei_linked_tag_and_widgets_t *searched_in, ei_widget_t *widget);

// append a link in linked list at head
// If link already in the list, returns it
// Otherwise returns the newly inserted link
// Return NULL if failure
ei_linked_tag_and_widgets_t *ei_linked_tag_and_widgets_append(ei_linked_tag_and_widgets_t **linked_list, ei_linked_tag_and_widgets_t *to_append);

// find a specific tag in the linked list and returns it if found. Otherwise returns NULL;
ei_linked_tag_and_widgets_t *ei_linked_tag_and_widgets_find(ei_linked_tag_and_widgets_t *entry_point, ei_tag_t tag);

// free the entire linked list
void ei_linked_tag_and_widgets_destroy(ei_linked_tag_and_widgets_t *first_link);


/* **** ITERATORS **** */

void *ei_it_linked_tag_and_widgets_next(struct ei_iterator_t *it);
ei_linked_tag_and_widgets_t *ei_it_linked_tag_and_widgets_get(struct ei_iterator_t *it);
struct ei_iterator_t ei_it_linked_tag_and_widgets_begin(ei_linked_tag_and_widgets_t *iterable);


#endif
