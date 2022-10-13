#include <assert.h>
#include <stdio.h>

#include "internals/event_internals/linked_widget.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "internals/ei_widgetclass_internal.h"
#include "helpers/ei_memory.h"
#include "internals/event_internals/event_internal.h"

static ei_widget_t *root_widget = NULL;

void test_ei_widget_create();
void test_ei_widget_destroy();

int main()
{
    ei_frame_register_class();
    ei_widgetclass_t *widgetclass = ei_widgetclass_from_name("frame");
    // set defaults value
    root_widget = widgetclass->allocfunc();
    widgetclass->setdefaultsfunc(root_widget);

    // set everything to 0
    root_widget->wclass = widgetclass;
    root_widget->parent = NULL;
    root_widget->children_head = NULL;
    root_widget->children_tail = NULL;
    root_widget->next_sibling = NULL;
    root_widget->user_data = NULL;
    root_widget->destructor = NULL;
    root_widget->pick_id = 0;
    root_widget->pick_color = (ei_color_t *)verified_calloc(1, sizeof(ei_color_t));
    root_widget->geom_params = NULL;
    root_widget->requested_size = (ei_size_t){0, 0};
    root_widget->screen_location = (ei_rect_t){{0, 0}, root_widget->requested_size};
    root_widget->content_rect = &root_widget->screen_location;

    test_ei_widget_destroy();

    ei_widget_destroy(root_widget);
    ei_widgetclass_destroy(ei_widgetclass_get());
    return 0;
}

void test_ei_widget_destroy()
{
    ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
    ei_widget_destroy(widget);
}
