#include <assert.h>

#include "ei_event.h"
#include "helpers/ei_string.h"
#include "iterators/ei_linked_list.h"
#include "internals/event_internals/event_internal.h"
#include "internals/event_internals/linked_tag_and_widgets.h"
#include "iterators/ei_iterator.h"

// hash map
void ei_bind(ei_eventtype_t eventtype,
             ei_widget_t *widget,
             ei_tag_t tag,
             ei_callback_t callback,
             void *user_param)
{
    assert(((size_t)widget ^ (size_t)tag) && "Both widget and tag cannot be non-NULL or NULL at the same time.\n");

    if (eventtype <= ei_ev_none || eventtype >= ei_ev_last)
        return;

    ei_linked_tag_and_widgets_t *first_tag = get_event_array(eventtype);
    if (widget != NULL)
    {
        ei_tag_t tag = widget->wclass->name;
        ei_linked_tag_and_widgets_t *found = ei_linked_tag_and_widgets_find(first_tag, tag);
        if (found == NULL) // TODO should we create the tag if not found ?
            return;
        ei_linked_tag_and_widgets_register(found, widget, callback, user_param);
    }
    else if (STRCMP(tag, ==, "all")) // widget is NULL here
    {
        /* TODO peut etre parcourir l'arbre des widgets ce qui permettrait de ne pas polluer le ei_widget_create avec du code evenementiel
        il faudrait donc enlever ce qui suit */
        ei_iterator_t it = ei_it_begin(linked_tag_and_widgets, first_tag);
        for (; ei_it_valid(&it); ei_it_next(&it))
        {
            ei_linked_tag_and_widgets_t *linked = ei_it_get_t(linked_tag_and_widgets, &it);
            ei_linked_tag_and_widgets_register_tag_callback(linked, callback, user_param);
        }
        ei_it_destroy(&it);
    }
    else if (tag != NULL) // widget is NULL here
    {
        ei_linked_tag_and_widgets_t *found = NULL;
        if (first_tag == NULL)
        {
            ei_linked_tag_and_widgets_t *new = ei_linked_tag_and_widgets_create(tag);
            if (ei_linked_tag_and_widgets_append(&first_tag, new) == NULL)
            {
                ei_linked_tag_and_widgets_destroy(new);
                return;
            }
            set_event_array(eventtype, first_tag);
            found = first_tag;
        }
        else
        {
            found = ei_linked_tag_and_widgets_find(first_tag, tag);
            if (found == NULL)
            {
                ei_linked_tag_and_widgets_t *new = ei_linked_tag_and_widgets_create(tag);
                ei_linked_tag_and_widgets_append(&first_tag, new);
            }
        }
        ei_linked_tag_and_widgets_register_tag_callback(found, callback, user_param);
    }
}

/**
 * \brief	Unbinds a callback from an event type and widget or tag.
 *
 * @param	eventtype, widget, tag, callback, user_param
 *				All parameters must have the same value as when \ref ei_bind was
 *				called to create the binding.
 */
void ei_unbind(ei_eventtype_t eventtype,
               ei_widget_t *widget,
               ei_tag_t tag,
               ei_callback_t callback,
               void *user_param)
{
    assert(((size_t)widget ^ (size_t)tag) && "Both widget and tag cannot be non-NULL or NULL at the same time.\n");

    ei_linked_tag_and_widgets_t *first_tag = get_event_array(eventtype);
    if (first_tag == NULL)
        return;
    if (widget != NULL)
    {
        ei_tag_t tag = widget->wclass->name;
        ei_linked_tag_and_widgets_t *found = ei_linked_tag_and_widgets_find(first_tag, tag);
        if (found != NULL)
            ei_linked_tag_and_widgets_unregister_callback(found, widget, callback, user_param);
    }
    else if (STRCMP(tag, ==, "all")) // widget is NULL here
    {
        ei_iterator_t it = ei_it_begin(linked_tag_and_widgets, first_tag);
        for (; ei_it_valid(&it); ei_it_next(&it))
        {
            ei_linked_tag_and_widgets_t *linked_tag = ei_it_get_t(linked_tag_and_widgets, &it);
            ei_linked_tag_and_widgets_unregister_tag_callback(linked_tag, callback, user_param);
        }
        ei_it_destroy(&it);
    }
    else if (tag != NULL) // widget is NULL here
    {
        ei_linked_tag_and_widgets_t *found = ei_linked_tag_and_widgets_find(first_tag, tag);
        if (found != NULL)
            ei_linked_tag_and_widgets_unregister_tag_callback(found, callback, user_param);
    }
}