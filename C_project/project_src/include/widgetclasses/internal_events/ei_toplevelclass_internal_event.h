#ifndef EI_TOPLEVELCLASS_INTERNAL_EVENT_H
#define EI_TOPLEVELCLASS_INTERNAL_EVENT_H

#include "ei_types.h"
#include "ei_widget.h"

struct ei_widget_t;
struct ei_event_t;


ei_bool_t ei_toplevel_click_internal_event(struct ei_widget_t *widget, struct ei_event_t *event, void *user_param);


#endif