#include "internals/event_internals/linked_tag_and_widgets.h"
#include "iterators/ei_iterator.h"
#include "helpers/ei_string.h"
#include "helpers/ei_memory.h"
#include "iterators/ei_widget_tree_iterator.h"
#include "ei_application.h"
#include "internals/event_internals/linked_callback.h"

// create linked widget_and_tag from a tag 
ei_linked_tag_and_widgets_t *ei_linked_tag_and_widgets_create(ei_tag_t tag)
{
    if (tag == NULL)
        return NULL;
    ei_linked_tag_and_widgets_t *new = verified_calloc(1, sizeof(ei_linked_tag_and_widgets_t));
    new->tag = tag;
    new->associated_widgets = ei_hashmap_widget_create(10);
    new->next = NULL;
    new->tag_callbacks = NULL;
    return new;
}

// associate a widget with this tag.
// Callbacks order is Last in first out
// If widget "wgt" not already registered then it is inserted.
// If callback is not NULL, then it is inserted, no matter if it is already registered.
void ei_linked_tag_and_widgets_register(ei_linked_tag_and_widgets_t *link, ei_widget_t *widget, ei_callback_t callback, void *user_params)
{
    if (STRCMP(widget->wclass->name, !=, link->tag))
        return;
    ei_hashmap_widget_insert(link->associated_widgets, widget, callback, user_params);
}


void ei_linked_tag_and_widgets_register_tag_callback(ei_linked_tag_and_widgets_t *link, ei_callback_t callback, void *user_params)
{
    ei_linked_callback_t * linked_callback = ei_linked_callback_create(callback, user_params);
    ei_linked_callback_append(&link->tag_callbacks, linked_callback);
}

void ei_linked_tag_and_widgets_unregister_widget(ei_linked_tag_and_widgets_t *link, ei_widget_t *widget)
{
    ei_hashmap_widget_remove(link->associated_widgets, widget, NULL, NULL);
}

void ei_linked_tag_and_widgets_unregister_callback(ei_linked_tag_and_widgets_t *link, ei_widget_t *widget, ei_callback_t callback, void *user_params)
{
    ei_hashmap_widget_remove(link->associated_widgets, widget, callback, user_params);
}

void ei_linked_tag_and_widgets_unregister_tag_callback(ei_linked_tag_and_widgets_t *link, ei_callback_t callback, void *user_params)
{
    ei_linked_callback_remove(&link->tag_callbacks, callback, user_params);
}

ei_linked_callback_t *ei_linked_tag_and_widgets_get_tag_callbacks(ei_linked_tag_and_widgets_t *link)
{
    return link->tag_callbacks;
}

ei_linked_callback_t *ei_linked_tag_and_widgets_get_widget_callbacks(ei_linked_tag_and_widgets_t *searched_in, ei_widget_t *widget)
{
    if (searched_in == NULL)
        return NULL;
    ei_linked_widget_t *found = ei_hashmap_widget_find(searched_in->associated_widgets, widget);
    if (found == NULL)
        return NULL;
    return found->linked_callbacks;
}

// append a link in linked list at head
// If link already in the list, returns it
// Otherwise returns the newly inserted link
// Return NULL if failure
ei_linked_tag_and_widgets_t *ei_linked_tag_and_widgets_append(ei_linked_tag_and_widgets_t **linked_list, ei_linked_tag_and_widgets_t *to_append)
{
    if (to_append == NULL)
        return NULL;
    ei_iterator_t linked_it = ei_it_begin(linked_tag_and_widgets, *linked_list);
    for (; ei_it_valid(&linked_it); ei_it_next(&linked_it))
    {
        ei_linked_tag_and_widgets_t *result = ei_it_get_t(linked_tag_and_widgets, &linked_it);
        if (STRCMP(result->tag, ==, to_append->tag))
        {
            ei_it_destroy(&linked_it);
            return result;
        }
    }
    ei_it_destroy(&linked_it);
    to_append->next = *linked_list;
    *linked_list = to_append;
    return to_append;
}

// find a specific tag in the linked list and returns it if found. Otherwise returns NULL;
ei_linked_tag_and_widgets_t *ei_linked_tag_and_widgets_find(ei_linked_tag_and_widgets_t *entry_point, ei_tag_t tag)
{
    ei_iterator_t linked_it = ei_it_begin(linked_tag_and_widgets, entry_point);
    for (; ei_it_valid(&linked_it); ei_it_next(&linked_it))
    {
        ei_linked_tag_and_widgets_t *link = ei_it_get_t(linked_tag_and_widgets, &linked_it);
        if (STRCMP(link->tag, ==, tag))
        {
            ei_it_destroy(&linked_it);
            return link;
        }
    }
    ei_it_destroy(&linked_it);
    return NULL;
}

// free the entire linked list
void ei_linked_tag_and_widgets_destroy(ei_linked_tag_and_widgets_t *first_link)
{
    for (ei_linked_tag_and_widgets_t *it = first_link; it != NULL;)
    {
        ei_linked_tag_and_widgets_t *tmp = it->next;
        ei_hashmap_widget_destroy(it->associated_widgets);
        free(it);
        it = tmp;
    }
}

LIST_ITERATOR(ei_linked_tag_and_widgets_t, linked_tag_and_widgets)