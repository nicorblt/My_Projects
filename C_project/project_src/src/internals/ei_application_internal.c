#include <ei_types.h>
#include "helpers/ei_memory.h"
#include "ei_application.h"
#include "hw_interface.h"
#include "helpers/ei_geometry_helpers.h"


void ei_invalidate_rect_from_widget(struct ei_widget_t *widget){
    ei_rect_t root_surface = hw_surface_get_rect(ei_app_root_surface());
    ei_rect_t invalidate_rect = intersect_rect(&widget->screen_location, &root_surface);
    if (widget->parent != NULL){
        invalidate_rect = intersect_rect(widget->parent->content_rect,  &widget->screen_location);
    }
    ei_app_invalidate_rect(&invalidate_rect);

}