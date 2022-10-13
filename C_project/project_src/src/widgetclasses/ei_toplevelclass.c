#include <string.h>

#include "ei_widget.h"
#include "widgets/ei_toplevel.h"
#include "helpers/ei_memory.h"
#include "ei_utils.h"
#include "helpers/ei_geometry_helpers.h"
#include "ei_event.h"
#include "helpers/ei_draw_helpers.h"
#include "widgetclasses/config/ei_toplevelclass_config.h"
#include "widgetclasses/internal_events/ei_toplevelclass_internal_event.h"

void ei_toplevel_releaser(ei_widget_t *widget)
{
    free(((ei_toplevel_t *)widget)->title);
}

ei_widget_t *ei_toplevel_allocator(void)
{
    ei_toplevel_t *new_toplevel = verified_calloc(1, sizeof(ei_toplevel_t));
    return (ei_widget_t *)new_toplevel;
}

void ei_toplevel_setdefaults(ei_widget_t *toplevel)
{
    ei_toplevel_t *ptr = (ei_toplevel_t *)toplevel;
    ptr->color = ei_default_background_color;
    ptr->closable = EI_TRUE;
    ptr->title = verified_calloc(9, sizeof(char));
    strncpy(ptr->title, "Toplevel", 9);
    ptr->title_font = ei_default_font;
    ptr->axis_set = ei_axis_both;
    ptr->border_width = 4;
    ptr->min_size = ei_size(160, 120);
    ptr->widget.requested_size = ei_size(320, 240);
    ptr->widget.content_rect = verified_calloc(1, sizeof(ei_rect_t));
}

void ei_toplevel_drawer(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    // Divide the widget in two side, the top strip and the lower frame.
    int border_width = toplevel->border_width;
    int height = widget->screen_location.size.height;
    int width = widget->screen_location.size.width;
    int topleft_x = widget->screen_location.top_left.x;
    int topleft_y = widget->screen_location.top_left.y;
    int botright_x = topleft_x + width - 1;
    int botright_y = topleft_y + height - 1;

    // Defintion af all point
    ei_point_t point_E = {botright_x, topleft_y + k_toplevel_strip_height};
    ei_point_t point_F = {topleft_x, topleft_y + k_toplevel_strip_height};
    ei_point_t point_G = {topleft_x, botright_y};
    ei_point_t point_H = {botright_x, botright_y};
    ei_point_t point_I = {botright_x - border_width, topleft_y + k_toplevel_strip_height};
    ei_point_t point_J = {botright_x - border_width, botright_y - border_width};
    ei_point_t point_K = {topleft_x + border_width, botright_y - border_width};
    ei_point_t point_L = {topleft_x + border_width, topleft_y + k_toplevel_strip_height};

    // Print the top strip.
    ei_point_t center_top_left = {topleft_x + k_radius, topleft_y + k_radius};
    ei_point_t center_top_right = {botright_x - k_radius, topleft_y + k_radius};

    ei_linked_point_t *strip = arc(center_top_left, k_radius, M_PI, M_PI / 2);         // add A->B
    strip = ei_linked_point_join(arc(center_top_right, k_radius, M_PI / 2, 0), strip); // add B->C->D
    strip = ei_linked_point_add(strip, point_E);
    strip = ei_linked_point_add(strip, point_F);
    ei_draw_polygon(surface, strip, k_color_border, clipper);

    // Display border
    ei_linked_point_t *border = NULL;
    border = ei_linked_point_add(border, point_F);
    border = ei_linked_point_add(border, point_G);
    border = ei_linked_point_add(border, point_H);
    border = ei_linked_point_add(border, point_E);
    border = ei_linked_point_add(border, point_I);
    border = ei_linked_point_add(border, point_J);
    border = ei_linked_point_add(border, point_K);
    border = ei_linked_point_add(border, point_L);
    border = ei_linked_point_add(border, point_F);
    ei_draw_polygon(surface, border, k_color_border, clipper);

    // Fill the center
    ei_rect_t *inner_clipper = widget->content_rect;
    ei_fill(surface, &toplevel->color, inner_clipper);

    // Display toplevel title
    if (toplevel->title != NULL)
    {
        ei_surface_t text_surface = hw_text_create_surface(toplevel->title, toplevel->title_font, k_head_strip_text_color);
        ei_size_t text_size = hw_surface_get_size(text_surface);
        ei_point_t where = {topleft_x + (int)(9 * k_toplevel_strip_height >> 3), topleft_y + (int)(k_toplevel_strip_height / 2 - text_size.height / 2)};
        ei_draw_text(surface, &where, toplevel->title, toplevel->title_font, k_head_strip_text_color, clipper);
    }

    // Draw pick surface
    ei_draw_polygon(pick_surface, strip, *widget->pick_color, clipper);
    ei_rect_t clipper_lower_frame = *clipper;
    clipper_lower_frame.top_left = point_F;
    clipper_lower_frame.size.height -= k_toplevel_strip_height;
    ei_fill(pick_surface, widget->pick_color, &clipper_lower_frame);

    // Display resize button
    ei_point_t tl_bottom_right_rectangle;
    tl_bottom_right_rectangle.x = botright_x - k_bottom_right_rectangle_width;
    tl_bottom_right_rectangle.y = botright_y - k_bottom_right_rectangle_width;
    ei_size_t size_bottom_right_rectangle = {k_bottom_right_rectangle_width, k_bottom_right_rectangle_width};
    ei_rect_t clipper_bottom_right_rectangle = {tl_bottom_right_rectangle, size_bottom_right_rectangle};
    ei_fill(surface, &k_color_border, &clipper_bottom_right_rectangle);
    ei_fill(pick_surface, widget->pick_color, &clipper_bottom_right_rectangle);

    // Display close button
    ei_rect_t clipper_close_button = {(ei_point_t){topleft_x + toplevel_close_button_x - k_toplevel_close_button_corner_radius, topleft_y + toplevel_close_button_y - k_toplevel_close_button_corner_radius }, k_toplevel_close_button_size};
    ei_color_t lower_color = modify_rgba_lightness(k_strip_button_color, -0.5);
    ei_color_t upper_color = modify_rgba_lightness(k_strip_button_color, 0.5);
    ei_linked_point_t *lower_frame = rounded_frame_borders(clipper_close_button, k_toplevel_close_button_corner_radius, "lower", k_close_border_width);
    ei_linked_point_t *upper_frame = rounded_frame_borders(clipper_close_button, k_toplevel_close_button_corner_radius, "upper", k_close_border_width);
    ei_linked_point_t *inner_frame = arc((ei_point_t){topleft_x + toplevel_close_button_x, topleft_y + toplevel_close_button_y}, k_toplevel_close_button_corner_radius-k_close_border_width, 0, 2*M_PI);
    ei_draw_polygon(surface, inner_frame, k_strip_button_color, &clipper_close_button);
    ei_draw_polygon(surface, lower_frame, lower_color, &clipper_close_button);
    ei_draw_polygon(surface, upper_frame, upper_color, &clipper_close_button);

    // ei_callback_t close_callback = ei_toplevel_close_button_internal_quit_event;

    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);
}

ei_bool_t ei_toplevel_click_on_close_button(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
    ei_point_t point = {event->param.mouse.where.x, event->param.mouse.where.y};
    uint32_t dist_to_center_of_close_button = sqrt((point.x -  widget->screen_location.top_left.x - toplevel_close_button_x)^2 + (point.y -  widget->screen_location.top_left.y - toplevel_close_button_y)^2);
    if (dist_to_center_of_close_button < k_toplevel_close_button_corner_radius)
    {
        return EI_TRUE;
    }
    return EI_FALSE;
}

void ei_toplevel_geomnotifier(ei_widget_t *toplevel)
{
    ei_rect_t inner_clipper;
    ei_point_t screen_pos = toplevel->screen_location.top_left;
    int border_width = ((ei_toplevel_t *)toplevel)->border_width;
    inner_clipper.top_left = (ei_point_t){screen_pos.x + border_width, screen_pos.y + k_toplevel_strip_height};
    inner_clipper.size.width = toplevel->requested_size.width - 2 * border_width;
    inner_clipper.size.height = toplevel->requested_size.height - border_width - k_toplevel_strip_height;
	*toplevel->content_rect = inner_clipper;
    ei_invalidate_rect_from_widget(toplevel);
}