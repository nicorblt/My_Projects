#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_event.h"
#include "helpers/ei_geometry_helpers.h"
#include "helpers/ei_memory.h"
#include "hw_interface.h"
#include "iterators/ei_widget_tree_iterator.h"
#include "internals/ei_widgetclass_internal.h"
#include "internals/ei_geometrymanager_internal.h"
#include "internals/ei_picking_internal.h"
#include "internals/event_internals/event_internal.h"
#include "helpers/ei_math.h"

#include <assert.h>
#include <stdio.h>

/**
 * @brief   Defines the default root widget of type ei_widget_t*.
 * This widget is the root of the tree composed of all the other widgets like windows, frames, buttons.
 * It is the only widget that has no parent.
 * Is purpose is to offer a *unique* point of access to the hierarchy of widgets and receive defaults events when they're not associated to any other interacteur.
 * First value is NULL and will be modified over the programm.
 */
static ei_widget_t *root_widget = NULL;

ei_bool_t running = EI_TRUE;

/**
 * @brief   Defines the linked list of \ref ei_rect_t of type ei_linked_rect_t.
 * This linked list is to store the rectangles of the widgets to be drawn.
 * For example we use this list in \ref hw_surface_update_rects to update the surface.
 * By default, the list contains only the rect of size 0 placed in (0,0) with no next rectangles.
 */
static ei_linked_rect_t invalidated_rects = {{{0, 0}, {0, 0}}, NULL};

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    // init the hardware and create the main window
    hw_init();
    ei_surface_t main_window = hw_create_window(main_window_size, fullscreen);

    // create an array for the events
    create_event_array();
    // register geometry managers
    ei_register_placer_manager();

    // register all classes of widget
    ei_frame_register_class();
    ei_toplevel_register_class();
    ei_button_register_class();
    ei_radiobutton_register_class();

    // register classes to the event manager
    register_widgetclasses_to_all_events();

    // creates root widget manually bec
    ei_widgetclass_t *widgetclass = ei_widgetclass_from_name("frame");
#ifndef RELEASE
    assert(widgetclass != NULL && "Root widgetclass is NULL.");
#endif
    // set defaults value
    root_widget = widgetclass->allocfunc();
    widgetclass->setdefaultsfunc(root_widget);
    // set everything to 0
    root_widget->wclass = widgetclass;
    root_widget->parent = NULL;
    root_widget->children_head = NULL;
    root_widget->children_tail = NULL;
    root_widget->next_sibling = NULL;
    root_widget->user_data = main_window;
    // init pick_surface for picking
    ei_app_pick_surface_create(ei_app_root_surface());
    root_widget->destructor = NULL;
    root_widget->pick_id = 0xff000000;
    root_widget->pick_color = ei_pick_color_from_pick_id(root_widget->pick_id);
    root_widget->geom_params = NULL;
    root_widget->requested_size = hw_surface_get_size(main_window);
    root_widget->screen_location = (ei_rect_t){{0, 0}, root_widget->requested_size};
    // if the widgetclass of root_widget make content_rect differs from screen_location
    free(root_widget->content_rect);
    root_widget->content_rect = &root_widget->screen_location;

    // Register the widget to the event manager, so that it can be later found by a tag.
    ei_register_widget_to_all_events(root_widget);

}

void ei_app_free(void)
{
    // hw_quit releases the root surface, do not call surface_free on it !
    ei_app_pick_surface_destroy();                        // free the picking surface
    ei_widget_destroy(root_widget);                       // free the root widget
    ei_linked_rect_destroy(invalidated_rects.next);       // free every invalidated rect remaining
    ei_widgetclass_destroy(ei_widgetclass_get());         // free every widgetclasses
    ei_geometrymanager_destroy(ei_geometrymanager_get()); // free every geometry managers
    destroy_event_array();                                // free the event array and every associated resources.
    hw_quit();                                            // release the handle over every hardware resources
}

void ei_app_run(void)
{
    while (running)
    {
        // DEPTH first search throughout the widget tree
        ei_widget_t *root_widget = ei_app_root_widget();
        ei_widget_t sent = *root_widget;
        sent.children_head = sent.children_tail = root_widget;
        root_widget->parent = &sent;
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = ei_app_pick_surface();
        ei_iterator_t wgt_it = ei_it_begin(widget_tree, root_widget);
        for (; ei_it_valid(&wgt_it); ei_it_next(&wgt_it))
        {
            ei_widget_t *wgt = ei_it_get_t(widget_tree, &wgt_it);
            
            ei_rect_t clipper = intersect_rect(wgt->parent->content_rect, &wgt->screen_location);
            wgt->wclass->drawfunc(wgt, root_surface, pick_surface, &clipper);
        }
        root_widget->parent = NULL;
        hw_surface_update_rects(root_surface, &invalidated_rects);

        /* We always conserve the first empty rect of the linked list.
        It is of no consequence on the performances because since the rectangle is empty, no pixel will be updated.
        */
        ei_linked_rect_destroy(invalidated_rects.next);
        invalidated_rects.next = NULL;

        ei_event_t event;
        double starting_date = hw_now();
        hw_event_schedule_app(5, &starting_date); // at least 144 Hz max
        do
        {
            // double b4_event = hw_now();
            hw_event_wait_next(&event);
            ei_manage_event(&event);
        // printf("Time to manage the event in sec: %f\n", hw_now() - b4_event);
        } while(hw_now() - starting_date < 0.0045);
    }
}

ei_bool_t ei_can_merge_rect(ei_rect_t rect1, ei_rect_t rect2)
{
    int32_t minimal_area = MIN(rect1.size.height * rect1.size.width, rect2.size.height * rect2.size.width);
    ei_rect_t intersected_rect = intersect_rect (&rect1, &rect2);
    if (minimal_area / 2 < intersected_rect.size.height * intersected_rect.size.width)
        return EI_TRUE;
    return EI_FALSE;    
}

ei_rect_t ei_merge_rect(ei_rect_t rect1, ei_rect_t rect2)
{
    ei_point_t top_left_merged = {MIN(rect1.top_left.x, rect2.top_left.x), MIN(rect1.top_left.y, rect2.top_left.y)};

    ei_rect_t intersected_rect = intersect_rect (&rect1, &rect2);
    ei_size_t size_merged = {rect1.size.width + rect2.size.width - intersected_rect.size.width, rect1.size.height + rect2.size.height - intersected_rect.size.height };
    
    ei_rect_t merged_rect = {top_left_merged, size_merged};
    return merged_rect;
}

ei_rect_t ei_merge_invalidate_rect(ei_rect_t *rect)
{
    ei_rect_t merged_rect = *rect;
    ei_bool_t flag_finish = EI_FALSE;
    ei_bool_t flag_merge = EI_FALSE;
    while(!flag_finish){    
        ei_linked_rect_t sent = {(ei_rect_t){(ei_point_t){0,0}, (ei_size_t){0,0}}, &invalidated_rects};
	    ei_linked_rect_t *current = &sent;
        while (!flag_merge && current->next != NULL)
        {
            if (ei_can_merge_rect(merged_rect, current->next->rect))
            {
                flag_merge = EI_TRUE;
                merged_rect = ei_merge_rect(merged_rect, current->next->rect);
                current->next = current->next->next;
            }
            else
            {
                current = current->next;
            }
        }
        if (current->next == NULL)
        {
            flag_finish = EI_TRUE;
        }
        flag_merge = EI_FALSE;
    }
    return(merged_rect);
}

void ei_app_invalidate_rect(ei_rect_t *rect)
{
    ei_rect_t merged_invalidate_rect = ei_merge_invalidate_rect(rect);
    ei_linked_rect_t *tmp = invalidated_rects.next;
    invalidated_rects.next = verified_calloc(1, sizeof(ei_linked_rect_t));
    invalidated_rects.next->rect = merged_invalidate_rect;
    invalidated_rects.next->next = tmp;
}

void ei_app_quit_request(void)
{
    running = EI_FALSE;
}

ei_widget_t *ei_app_root_widget(void)
{
    return root_widget;
}

ei_surface_t ei_app_root_surface(void)
{
    return ei_app_root_widget()->user_data;
}
