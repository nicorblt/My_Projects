#include "widgetclasses/ei_frameclass.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "widgets/ei_frame.h"
#include "helpers/ei_geometry_helpers.h"
#include "helpers/ei_memory.h"
#include "helpers/ei_draw_helpers.h"
#include "helpers/ei_string.h"
#include "ei_types.h"
#include "helpers/ei_math.h"
#include "ei_application.h"
#include "internals/ei_application_internal.h"

static void ei_frame_draw_relief(ei_widget_t *widget, ei_surface_t surface, const ei_rect_t *clipper)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    int height = widget->screen_location.size.height;
    int width = widget->screen_location.size.width;
    int topleft_x = widget->screen_location.top_left.x;
    int topleft_y = widget->screen_location.top_left.y;
    int botright_x = topleft_x + width - 1;
    int botright_y = topleft_y + height - 1;
    ei_relief_t relief = frame->relief;

    ei_color_t color = frame->color;

    ei_color_t darkest_color = modify_rgba_lightness(color, 0.3);
    ei_color_t lightest_color = modify_rgba_lightness(color, -0.3);

    ei_point_t point_A = {topleft_x, botright_y};
    ei_point_t point_B = {topleft_x, topleft_y};
    ei_point_t point_C = {botright_x, topleft_y};
    ei_point_t point_D = {botright_x, botright_y};
    ei_point_t point_E = {topleft_x + (int)(height / 2), topleft_y + (int)(height / 2)};
    ei_point_t point_F = {botright_x - (int)(height / 2), topleft_y + (int)(height / 2)};

    if (width < height)
    { // vertical frame
        point_E.x = topleft_x + (int)(width / 2);
        point_E.y = botright_y - (int)(width / 2);
        point_F.x = topleft_x + (int)(width / 2);
        point_F.y = topleft_y + (int)(width / 2);
    }

    // create the upper (or left) side (A->B->C->F->E->A)
    ei_linked_point_t *upper_side = NULL;
    upper_side = ei_linked_point_add(upper_side, point_A);
    upper_side = ei_linked_point_add(upper_side, point_E);
    upper_side = ei_linked_point_add(upper_side, point_F);
    upper_side = ei_linked_point_add(upper_side, point_C);
    upper_side = ei_linked_point_add(upper_side, point_B);
    upper_side = ei_linked_point_add(upper_side, point_A);

    // create the lower (or right) side (A->D->C->F->E->A)
    ei_linked_point_t *lower_side = NULL;
    lower_side = ei_linked_point_add(lower_side, point_A);
    lower_side = ei_linked_point_add(lower_side, point_E);
    lower_side = ei_linked_point_add(lower_side, point_F);
    lower_side = ei_linked_point_add(lower_side, point_C);
    lower_side = ei_linked_point_add(lower_side, point_D);
    lower_side = ei_linked_point_add(lower_side, point_A);

    if (relief == ei_relief_raised)
    {
        ei_draw_polygon(surface, lower_side, lightest_color, clipper);
        ei_draw_polygon(surface, upper_side, darkest_color, clipper);
    }
    else if (relief == ei_relief_sunken)
    {
        ei_draw_polygon(surface, lower_side, darkest_color, clipper);
        ei_draw_polygon(surface, upper_side, lightest_color, clipper);
    }
}

// TODO à check les fuites mémoires. (benjamin)

ei_widget_t *ei_frame_allocator()
{
    ei_frame_t *new_frame = verified_calloc(1, sizeof(ei_frame_t));
    return (ei_widget_t *)new_frame;
}

void ei_frame_releaser(ei_widget_t *widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;
    free(frame->text); // Heap allocated (see frame_set_text)
    if (frame->bg_image != NULL)
        hw_surface_free(frame->bg_image);
}

void ei_frame_drawer(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    ei_fill(pick_surface, widget->pick_color, clipper);
    ei_rect_t inner_clipper = *clipper;

    if (frame->relief != ei_relief_none && STRCMP(widget->wclass->name, ==, "frame"))
    {
        ei_frame_draw_relief(widget, surface, clipper);
        inner_clipper = intersect_rect(clipper, frame->widget.content_rect); // nouveau
    }

    ei_surface_t root_surface = ei_app_root_surface(); // TODO on fill tout le frame dans tout les cas, à réfléchir (peut etre fill que là ou y a pas d'image)
    // TODO fill que si frame.size > bg_image.size
    if (surface == root_surface)
    {
        ei_size_t root_size = hw_surface_get_size(root_surface);
        ei_surface_t for_transparency = hw_surface_create(root_surface, root_size, EI_TRUE);
        ei_fill(for_transparency, &frame->color, &inner_clipper);
        ei_copy_surface(root_surface, NULL, for_transparency, NULL, EI_TRUE);
        hw_surface_free(for_transparency);
    }
    else
    {
        ei_fill(surface, &frame->color, &inner_clipper);
    }

    if (frame->bg_image != NULL)
    {
        ei_rect_t dst_rect = (frame->bg_image_aspect.area != NULL) ? *frame->bg_image_aspect.area : hw_surface_get_rect(frame->bg_image);
        dst_rect.top_left = ei_get_coords_from_anchor(frame->bg_image_aspect.anchor, &widget->screen_location, frame->bg_image_aspect.area->size);
        ei_rect_t clipped_destination = intersect_rect(&inner_clipper, &dst_rect);
        ei_rect_t src_rect = (frame->bg_image_aspect.area != NULL) ? *frame->bg_image_aspect.area : hw_surface_get_rect(frame->bg_image);
        src_rect.top_left = clipped_destination.top_left;
        src_rect.size.width = MIN(src_rect.size.width, clipped_destination.size.width);
        src_rect.size.height = MIN(src_rect.size.height, clipped_destination.size.height);
        ei_rect_t intersection = intersect_rect(&clipped_destination, &src_rect);
        ei_rect_t img_container = (frame->bg_image_aspect.area != NULL) ? *frame->bg_image_aspect.area : hw_surface_get_rect(frame->bg_image);
        src_rect.top_left = ei_get_coords_from_anchor(frame->bg_image_aspect.anchor, &img_container, src_rect.size);
        int success = ei_copy_surface(surface, &intersection, frame->bg_image, &src_rect, EI_TRUE);
#ifndef RELEASE
        if (!success)
        {
            puts("copy_surface failed while drawing bg_image (probably because frame->screen_location = {0,0,0,0})\n or because both sizes do not match.");
        }
#endif
    }

    // Draw the text
    if (frame->text != NULL)
    {
        ei_surface_t text_surface = hw_text_create_surface(frame->text, frame->text_aspect.font, frame->text_aspect.color);
        ei_size_t text_size = hw_surface_get_size(text_surface);

        ei_point_t where = ei_get_coords_from_anchor(frame->text_aspect.anchor, &inner_clipper, text_size);
        ei_rect_t text_clipper = intersect_rect(&inner_clipper, clipper);
        ei_draw_text(surface, &where, frame->text, frame->text_aspect.font, frame->text_aspect.color, &text_clipper);
    }

    // Unlock and update surface
    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);
}

void ei_frame_setdefaults(ei_widget_t *frame)
{
    ei_frame_t *ptr = (ei_frame_t *)frame;

    ptr->color = ei_default_background_color;
    ptr->border_width = 0;
    ptr->relief = ei_relief_none;
    ptr->text = NULL;
    ptr->text_aspect.font = ei_default_font;
    ptr->text_aspect.style = ei_style_normal;
    ptr->text_aspect.color = ei_font_default_color;
    ptr->text_aspect.anchor = ei_anc_center;
    ptr->bg_image = NULL;
    ptr->bg_image_aspect.area = NULL;
    ptr->bg_image_aspect.anchor = ei_anc_center;
    ptr->widget.content_rect = verified_calloc(1, sizeof(ei_rect_t));
    // ei_frame_geomnotifier(frame);
}

void ei_frame_geomnotifier(struct ei_widget_t *widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;
    int border_width = frame->border_width;
    *widget->content_rect = widget->screen_location;
    widget->content_rect->size.height -= border_width << 1;
    widget->content_rect->size.width -= border_width << 1;
    widget->content_rect->top_left.x += border_width;
    widget->content_rect->top_left.y += border_width;
    ei_invalidate_rect_from_widget(widget);
}