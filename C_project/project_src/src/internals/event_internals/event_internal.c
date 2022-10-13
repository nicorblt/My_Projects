#include <assert.h>

#include "ei_widget.h"
#include "internals/event_internals/event_internal.h"
#include "internals/event_internals/linked_callback.h"
#include "iterators/ei_iterator.h"
#include "iterators/ei_widget_tree_iterator.h"
#include "ei_application.h"
#include "helpers/ei_memory.h"
#include "internals/ei_widgetclass_internal.h"

#define NB_OF_EVENTTYPE (ei_ev_last -1)
static ei_linked_tag_and_widgets_t** ev_type_array = NULL;


void ei_register_widget_to_all_events(ei_widget_t *widget)
{
    ei_tag_t tag = widget->wclass->name;
    for (size_t ev_type = 0; ev_type < NB_OF_EVENTTYPE; ev_type++)
    {
        ei_linked_tag_and_widgets_t *matching_tag = ei_linked_tag_and_widgets_find(get_event_array(ev_type+1), tag);
        if (matching_tag == NULL)
            return;
        ei_linked_tag_and_widgets_register(matching_tag, widget, NULL, NULL);
    }
}

void ei_unregister_widget_to_all_events(ei_widget_t *widget)
{
    ei_tag_t tag = widget->wclass->name;
    for (size_t ev_type = 0; ev_type < NB_OF_EVENTTYPE; ev_type++)
    {
        ei_linked_tag_and_widgets_t *matching_tag = ei_linked_tag_and_widgets_find(get_event_array(ev_type+1), tag);
        if (matching_tag == NULL)
            return;
        ei_linked_tag_and_widgets_unregister_widget(matching_tag, widget);
    }
}

void register_widgetclasses_to_all_events()
{
    if (ev_type_array == NULL)
        return;
    // go throughout every registered widgetclasses and create an entry point
    for (size_t i=0; i < NB_OF_EVENTTYPE; i++)
    {
        ei_iterator_t widgetclass_it = ei_it_begin(widgetclass, ei_widgetclass_get());
        for (; ei_it_valid(&widgetclass_it); ei_it_next(&widgetclass_it))
        {
            ei_tag_t tag = ei_it_get_t(widgetclass, &widgetclass_it)->name;
            ei_linked_tag_and_widgets_t *new = ei_linked_tag_and_widgets_create(tag);
            ei_linked_tag_and_widgets_t *inserted = ei_linked_tag_and_widgets_append(&ev_type_array[i], new);
            if (inserted != new)
            {
                ei_linked_tag_and_widgets_destroy(new);
            }
        }
        ei_it_destroy(&widgetclass_it);
    }
}

void create_event_array()
{
    if (ev_type_array == NULL)
    {
        ev_type_array = verified_calloc(NB_OF_EVENTTYPE, sizeof(ei_linked_tag_and_widgets_t *));
    }
}

size_t get_event_array_length(void)
{
    return NB_OF_EVENTTYPE;
}

ei_linked_tag_and_widgets_t* get_event_array(ei_eventtype_t eventtype)
{
    if (ev_type_array != NULL && eventtype - 1 < NB_OF_EVENTTYPE && eventtype > ei_ev_none)
        return ev_type_array[eventtype - 1];
    return NULL;
}

void set_event_array(ei_eventtype_t eventtype, ei_linked_tag_and_widgets_t *head)
{
    assert(ev_type_array != NULL && (eventtype - 1) < NB_OF_EVENTTYPE && eventtype > ei_ev_none);
    ev_type_array[eventtype - 1] = head;
}

void destroy_event_array()
{
    for (size_t i = 0; i != NB_OF_EVENTTYPE; i++)
    {
        ei_linked_tag_and_widgets_destroy(ev_type_array[i]);
    }
    free(ev_type_array);
    ev_type_array = NULL;
}


void ei_call_callbacks(ei_event_t *event, ei_widget_t *widget)
{
    ei_eventtype_t eventtype = event->type;
    ei_tag_t tag = widget->wclass->name;
    ei_linked_tag_and_widgets_t *entry_point = get_event_array(eventtype);
    if (entry_point == NULL)
        return;
    ei_linked_tag_and_widgets_t *matching_tag = ei_linked_tag_and_widgets_find(entry_point, tag);
    if (matching_tag == NULL)
        return;

    ei_iterator_t callback_it = ei_it_begin(linked_callback, ei_linked_tag_and_widgets_get_tag_callbacks(matching_tag));
    for(; ei_it_valid(&callback_it); ei_it_next(&callback_it))
    {
        ei_linked_callback_t *linked_cb = ei_it_get_t(linked_callback, &callback_it);
        if (linked_cb->callback(widget, event, linked_cb->user_params) == EI_TRUE)
        {
            // event was consumed by the callback
            ei_it_destroy(&callback_it);
            return;
        }
    }
    ei_it_destroy(&callback_it);

    callback_it = ei_it_begin(linked_callback, ei_linked_tag_and_widgets_get_widget_callbacks(matching_tag, widget));
    for (; ei_it_valid(&callback_it); ei_it_next(&callback_it))
    {
        ei_linked_callback_t *linked_cb = ei_it_get_t(linked_callback, &callback_it);
        if (linked_cb->callback(widget, event, linked_cb->user_params) == EI_TRUE)
        {
            // event was consumed by the callback
            ei_it_destroy(&callback_it);
            return;
        }
    }
    ei_it_destroy(&callback_it);
}


static void _mouse_event(ei_event_t *event)
{
    ei_point_t mouse_pos = event->param.mouse.where;

    ei_widget_t *clicked_on = ei_widget_pick(&mouse_pos);

    if (clicked_on != NULL)
    {
        ei_call_callbacks(event, clicked_on);
    }
}

static void _key_event(ei_event_t *event)
{
    ei_iterator_t widget_it = ei_it_begin(widget_tree, ei_app_root_widget());
    for (; ei_it_valid(&widget_it); ei_it_next(&widget_it))
    {
        ei_widget_t *widget = ei_it_get_t(widget_tree, &widget_it);
        ei_call_callbacks(event, widget);
    }
    ei_it_destroy(&widget_it);
}

void ei_manage_event(ei_event_t * event)
{
    switch(event->type)
    {
        case ei_ev_app: ///< An application event, created by \ref hw_event_post_app.
        case ei_ev_exposed: ///< The application's window has been exposed and should be redrawn.
            break;
        case ei_ev_keydown:
        case ei_ev_keyup:
            _key_event(event);
            break;
        case ei_ev_mouse_buttondown:
        case ei_ev_mouse_buttonup:
        case ei_ev_mouse_move:
            _mouse_event(event);
            break;
        default:
            break;   
    }
}