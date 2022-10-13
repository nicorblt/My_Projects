#ifndef EVENT_INTERNALS_EVENT_INTERNAL_H
#define EVENT_INTERNALS_EVENT_INTERNAL_H

#include "ei_event.h"
#include "internals/event_internals/linked_tag_and_widgets.h"

struct ei_widget_t;


void create_event_array();
void register_widgetclasses_to_all_events();
ei_linked_tag_and_widgets_t* get_event_array(ei_eventtype_t eventtype);
void set_event_array(ei_eventtype_t eventtype, ei_linked_tag_and_widgets_t *head);
size_t get_event_array_length(void);
void destroy_event_array();

void ei_register_widget_to_all_events(struct ei_widget_t *widget);
void ei_unregister_widget_to_all_events(struct ei_widget_t *widget);
void ei_call_callbacks(ei_event_t *event, struct ei_widget_t *widget);
void ei_manage_event(ei_event_t * event);

#endif