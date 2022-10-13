#include "ei_widgetclass.h"
#include "widgetclasses/ei_buttonclass.h"
#include "widgetclasses/ei_frameclass.h"
#include "ei_widget.h"
#include "widgets/ei_button.h"
#include "helpers/ei_memory.h"
#include "internals/ei_application_internal.h"
#include "helpers/ei_geometry_helpers.h"
#include "helpers/ei_draw_helpers.h"
#include "ei_event.h"
#include "internals/ei_application_internal.h"

ei_widget_t *ei_button_allocator()
{
    ei_button_t *new_button = verified_calloc(1, sizeof(ei_button_t));
    return (ei_widget_t *)new_button;
}

void ei_button_releaser(ei_widget_t *widget)
{
    ei_widgetclass_t *frameclass = ei_widgetclass_from_name("frame");
    frameclass->releasefunc(widget);
}

void ei_button_drawer(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_frame_t *frame = (ei_frame_t *)widget;
    ei_button_t *button = (ei_button_t *)widget;
    ei_linked_point_t *lower_side;
    ei_linked_point_t *upper_side;

    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    ei_color_t button_color = frame->color;
    ei_rect_t rect = widget->screen_location;
    int radius = button->corner_radius;
    int border_width = frame->border_width;

    if (frame->bg_image != NULL)
    { // if we put an image in the button
        // first draw the center surface with frame_drawer
        ei_point_t included_top_left = {rect.top_left.x + (int)(radius * (1 - sqrt(2) / 2)),
                                        rect.top_left.y + (int)(radius * (1 - sqrt(2) / 2))};
        ei_size_t included_size = {rect.size.width - 2 * (int)(radius * (1 - sqrt(2) / 2)) - 1,
                                   rect.size.height - 2 * (int)(radius * (1 - sqrt(2) / 2)) - 1};
        ei_rect_t included_rect = {included_top_left, included_size};
        ei_rect_t intersected_center_clipper = intersect_rect(clipper, &included_rect);

        ei_frame_drawer(widget, surface, pick_surface, &intersected_center_clipper);

        // write over the placed frame the relief

        lower_side = rounded_frame_borders(rect, radius, "lower", border_width);
        upper_side = rounded_frame_borders(rect, radius, "upper", border_width);

        ei_linked_point_t *all_side = rounded_frame(rect, radius, "all");
        ei_draw_polygon(pick_surface, all_side, *widget->pick_color, clipper);
    }
    else
    {
        lower_side = rounded_frame(rect, radius, "lower");
        upper_side = rounded_frame(rect, radius, "upper");
        ei_draw_polygon(pick_surface, lower_side, *widget->pick_color, clipper);
        ei_draw_polygon(pick_surface, upper_side, *widget->pick_color, clipper);
    }

    ei_relief_t state = frame->relief;

    ei_color_t upper_color = modify_rgba_lightness(button_color, 0.3);
    ei_color_t lower_color = modify_rgba_lightness(button_color, -0.3);

    if (state == ei_relief_sunken)
    {
        ei_color_t tmp = upper_color;
        upper_color = lower_color;
        lower_color = tmp;
    }
    ei_draw_polygon(surface, lower_side, lower_color, clipper);
    ei_draw_polygon(surface, upper_side, upper_color, clipper);

    if (frame->bg_image == NULL)
    {
        ei_rect_t bordered_rect;
        bordered_rect.top_left.x = rect.top_left.x + border_width;
        bordered_rect.top_left.y = rect.top_left.y + border_width;
        bordered_rect.size.height = rect.size.height - 2 * border_width;
        bordered_rect.size.width = rect.size.width - 2 * border_width;
        ei_linked_point_t *all_side = rounded_frame(bordered_rect, radius - border_width, "all");
        ei_draw_polygon(surface, all_side, frame->color, clipper);

        if (frame->text != NULL)
        {
            ei_surface_t text_surface = hw_text_create_surface(frame->text, frame->text_aspect.font, frame->text_aspect.color);
            ei_size_t text_size = hw_surface_get_size(text_surface);
            ei_point_t where = ei_get_coords_from_anchor(frame->text_aspect.anchor, &rect, text_size);
            ei_rect_t text_clipper = intersect_rect(widget->content_rect, clipper);
            ei_draw_text(surface, &where, frame->text, frame->text_aspect.font, frame->text_aspect.color, &text_clipper);
        }
    }
    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);
}

void ei_button_setdefaults(ei_widget_t *button)
{
    ei_button_t *ptr = (ei_button_t *)button;
    ei_widgetclass_t *frameclass = ei_widgetclass_from_name("frame");
    frameclass->setdefaultsfunc(button);

    ptr->frame.widget.user_data = NULL;
    ptr->frame.relief = ei_relief_raised;
    ptr->frame.border_width = k_default_button_border_width;
    ptr->callback = NULL;
    ptr->corner_radius = k_default_button_corner_radius;
}

void ei_button_geomnotifier(struct ei_widget_t *button)
{
    ei_frame_t *button_frame = (ei_frame_t *)button;
    int border_width = button_frame->border_width;
    *button->content_rect = button->screen_location;
    // Note : on ne prend pas en compte les coins arrondis : il faudrait une fonction ei_rect_t intersect_rect_polygon(ei_rect_t rect, ei_linked_point_t polygon) 
    ei_invalidate_rect_from_widget(button);
}

struct button_state
{
    ei_button_t *button;
    ei_relief_t original_relief;
    ei_relief_t previous_relief;
};

static ei_bool_t ei_button_until_release(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    struct button_state *state = (struct button_state *)user_param;
    if (widget == (ei_widget_t *)state->button)
        state->button->frame.relief = (state->original_relief == ei_relief_sunken) ? ei_relief_raised : ei_relief_sunken;
    else
        state->button->frame.relief = state->original_relief;

    if (state->button->frame.relief != state->previous_relief)
    {
        state->previous_relief = state->button->frame.relief;
        ei_invalidate_rect_from_widget(state->button);
    }
    return EI_FALSE;
}

static ei_bool_t ei_button_release(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    struct button_state *state = (struct button_state *)user_param;
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_button_release, user_param);
    ei_unbind(ei_ev_mouse_move, NULL, "all", ei_button_until_release, user_param);
    state->button->frame.relief = state->original_relief;
    ei_invalidate_rect_from_widget(state->button);
    free(state);
    return EI_FALSE;
}

ei_bool_t ei_button_internal_event(struct ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
    ei_button_t *btn = (ei_button_t *)widget;
    struct button_state *state = verified_calloc(1, sizeof(struct button_state));
    state->button = btn;
    state->original_relief = btn->frame.relief;
    state->previous_relief = btn->frame.relief;
    btn->frame.relief = (state->original_relief == ei_relief_sunken) ? ei_relief_raised : ei_relief_sunken;
    ei_invalidate_rect_from_widget(state->button);
    ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_button_release, state);
    ei_bind(ei_ev_mouse_move, NULL, "all", ei_button_until_release, state);
    return EI_FALSE;
}
