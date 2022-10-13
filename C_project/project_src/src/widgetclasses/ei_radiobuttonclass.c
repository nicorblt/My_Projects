#include "ei_widgetclass.h"
#include "widgetclasses/ei_buttonclass.h"
#include "widgetclasses/ei_radiobuttonclass.h"
#include "widgetclasses/ei_frameclass.h"
#include "ei_widget.h"
#include "widgets/ei_radiobutton.h"
#include "helpers/ei_memory.h"
#include "helpers/ei_geometry_helpers.h"
#include "helpers/ei_draw_helpers.h"
#include "ei_event.h"
#include "helpers/ei_math.h"

static double k_coeff_reduction = 0.8;
static int spacing_cst = 15;

ei_widget_t *ei_radiobutton_allocator()
{
    ei_radiobutton_t *new_radiobutton = verified_calloc(1, sizeof(ei_radiobutton_t));
    return (ei_widget_t *)new_radiobutton;
}

void ei_radiobutton_releaser(ei_widget_t *widget)
{

    ei_widgetclass_t *frameclass = ei_widgetclass_from_name("frame");
    frameclass->releasefunc(widget);
}

void ei_radiobutton_drawer(ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_radiobutton_t *radiobutton = (ei_radiobutton_t *)widget;
    ei_frame_t *frame = (ei_frame_t *)widget;

    hw_surface_lock(surface);
    hw_surface_lock(pick_surface);

    // TODO: prendre en compte le border_width ?

    ei_rect_t rect = widget->screen_location;
    ei_point_t top_left;

    int radius, size, width, height, base;
    int text_spacing;

    // Distinct cases according to the button's shape
    switch (radiobutton->shape.type)
    {
    case ei_shape_circle:
        radius = radiobutton->shape.properties.circle.radius;
        text_spacing = radius * 2;
        ei_point_t center = (ei_point_t){rect.top_left.x + radius, rect.top_left.y + radius};
        ei_linked_point_t *points_extern_circle = arc(center, radius, 0, 2 * M_PI);
        ei_draw_polygon(surface, points_extern_circle, frame->color, clipper);
        ei_draw_polygon(pick_surface, points_extern_circle, *widget->pick_color, clipper);

        if (radiobutton->selected == EI_TRUE)
        {
            int inner_radius = (int)(radius * k_coeff_reduction);
            ei_linked_point_t *points_intern_circle = arc(center, inner_radius, 0, 2 * M_PI);
            ei_draw_polygon(surface, points_intern_circle, radiobutton->fill_color, clipper);
        }

        break;
    case ei_shape_square:
        size = radiobutton->shape.properties.square.size;
        text_spacing = size;
        ei_linked_point_t *points_extern_square = ei_linked_point_square(size, rect.top_left);
        ei_draw_polygon(surface, points_extern_square, frame->color, clipper);
        ei_draw_polygon(pick_surface, points_extern_square, *widget->pick_color, clipper);

        if (radiobutton->selected == EI_TRUE)
        {
            int inner_size = (int)(size * k_coeff_reduction);
            top_left.x = rect.top_left.x + (int)((size - inner_size) / 2);
            top_left.y = rect.top_left.y + (int)((size - inner_size) / 2);
            ei_linked_point_t *points_intern_square = ei_linked_point_square(inner_size, top_left);
            ei_draw_polygon(surface, points_intern_square, radiobutton->fill_color, clipper);
        }

        break;
    case ei_shape_rectangle:
        width = radiobutton->shape.properties.rectangle.width;
        height = radiobutton->shape.properties.rectangle.height;
        text_spacing = width;
        ei_linked_point_t *points_extern_rectangle = ei_linked_point_rectangle((ei_size_t){width, height}, rect.top_left);
        ei_draw_polygon(surface, points_extern_rectangle, frame->color, clipper);
        ei_draw_polygon(pick_surface, points_extern_rectangle, *widget->pick_color, clipper);

        if (radiobutton->selected == EI_TRUE)
        {
            ei_size_t inner_size = {(int)(width * k_coeff_reduction), (int)(height * k_coeff_reduction)};
            top_left.x = rect.top_left.x + (int)((width - inner_size.width) / 2);
            top_left.y = rect.top_left.y + (int)((height - inner_size.height) / 2);
            ei_linked_point_t *points_intern_rectangle = ei_linked_point_rectangle(inner_size, top_left);
            ei_draw_polygon(surface, points_intern_rectangle, radiobutton->fill_color, clipper);
        }

        break;
    case ei_shape_diamond:
        size = radiobutton->shape.properties.diamond.size;
        text_spacing = size;
        ei_linked_point_t *points_extern_diamond = ei_linked_point_diamond(size, rect.top_left);
        ei_draw_polygon(surface, points_extern_diamond, frame->color, clipper);
        ei_draw_polygon(pick_surface, points_extern_diamond, *widget->pick_color, clipper);

        if (radiobutton->selected == EI_TRUE)
        {
            int inner_size = (int)(size * k_coeff_reduction);
            top_left.x = rect.top_left.x + (int)((size - inner_size) / 2);
            top_left.y = rect.top_left.y + (int)((size - inner_size) / 2);
            ei_linked_point_t *points_intern_diamond = ei_linked_point_diamond(inner_size, top_left);
            ei_draw_polygon(surface, points_intern_diamond, radiobutton->fill_color, clipper);
        }

        break;
    case ei_shape_triangle:
        base = radiobutton->shape.properties.triangle.base;
        height = radiobutton->shape.properties.triangle.height;
        text_spacing = base;
        ei_linked_point_t *points_extern_triangle = ei_linked_point_triangle((ei_size_t){base, height}, rect.top_left);
        ei_draw_polygon(surface, points_extern_triangle, frame->color, clipper);
        ei_draw_polygon(pick_surface, points_extern_triangle, *widget->pick_color, clipper);

        if (radiobutton->selected == EI_TRUE)
        {
            ei_size_t inner_size = {(int)(base * k_coeff_reduction), (int)(height * k_coeff_reduction)};
            top_left.x = rect.top_left.x + (int)((base - inner_size.width) / 2);
            top_left.y = rect.top_left.y + (int)((height - inner_size.height) / 2);
            ei_linked_point_t *points_intern_triangle = ei_linked_point_triangle(inner_size, top_left);
            ei_draw_polygon(surface, points_intern_triangle, radiobutton->fill_color, clipper);
        }

        break;
    default:
        break;
    }

    // Puts the text on the right side of the button

    if (frame->text != NULL)
    {
        ei_surface_t text_surface = hw_text_create_surface(frame->text, frame->text_aspect.font, frame->text_aspect.color);
        ei_size_t text_size = hw_surface_get_size(text_surface);
        ei_point_t where_anchor = ei_get_coords_from_anchor(frame->text_aspect.anchor, widget->content_rect, text_size);
        ei_point_t where = {where_anchor.x + text_spacing + spacing_cst, where_anchor.y};
        ei_rect_t text_rect = {where, {text_size.width, text_size.height}};
        ei_rect_t text_clipper = intersect_rect(&text_rect, widget->parent->content_rect);
        ei_draw_text(surface, &where, frame->text, frame->text_aspect.font, frame->text_aspect.color, &text_clipper);
        ei_fill(pick_surface, widget->pick_color, &text_clipper);
    }

    hw_surface_unlock(pick_surface);
    hw_surface_unlock(surface);
}

void ei_radiobutton_setdefaults(ei_widget_t *radiobutton)
{
    ei_radiobutton_t *ptr = (ei_radiobutton_t *)radiobutton;
    ei_widgetclass_t *frameclass = ei_widgetclass_from_name("frame");
    frameclass->setdefaultsfunc(radiobutton);

    memset(&ptr->shape.properties, 0, sizeof(ptr->shape.properties));

    ptr->selected = EI_FALSE;
    ptr->frame.widget.user_data = NULL;
    ptr->shape.type = ei_shape_circle;
    ptr->shape.properties.circle.radius = k_default_radiobutton_border_width;
    ptr->id = 0;
    ptr->callback = NULL;
    ptr->next = NULL;
    ptr->fill_color = ptr->frame.color;
    ptr->form = NULL;
}

void ei_radiobutton_register_class(void)
{
    ei_widgetclass_t *radiobuttonclass = verified_calloc(1, sizeof(ei_widgetclass_t));

    strncpy(radiobuttonclass->name, "radiobutton", 12);
    radiobuttonclass->allocfunc = ei_radiobutton_allocator;
    radiobuttonclass->releasefunc = ei_radiobutton_releaser;
    radiobuttonclass->drawfunc = ei_radiobutton_drawer;
    radiobuttonclass->setdefaultsfunc = ei_radiobutton_setdefaults;
    radiobuttonclass->geomnotifyfunc = ei_radiobutton_geomnotifier;

    ei_widgetclass_register(radiobuttonclass);
}

void ei_radiobutton_geomnotifier(struct ei_widget_t *radiobutton)
{
    // We have to notify all the buttons in the form :
    ei_radiobutton_form_t *form = ((ei_radiobutton_t *)radiobutton)->form;
    ei_radiobutton_t *current = form->head;
    ei_rect_t current_clipper;
    while (current != NULL)
    {
        current_clipper.top_left = current->frame.widget.screen_location.top_left;
        ei_surface_t text_surface = hw_text_create_surface(current->frame.text, current->frame.text_aspect.font, current->frame.text_aspect.color);
        ei_size_t text_size = hw_surface_get_size(text_surface);
        current_clipper.size.width = current->frame.widget.screen_location.size.width + spacing_cst + text_size.width;
        current_clipper.size.height = MAX(current->frame.widget.screen_location.size.height, text_size.height);
        hw_surface_free(text_surface);
        ((ei_widget_t *)current)->content_rect = &current_clipper;
        current = current->next;
    }
    ei_invalidate_rect_from_widget(radiobutton);
}

struct radiobutton_state
{
    ei_radiobutton_t *radiobutton;
    ei_bool_t selected;
};

/* static ei_bool_t ei_radiobutton_release(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    struct radiobutton_state *state = (struct radiobutton_state *)user_param;
    ei_unbind(ei_ev_mouse_buttonup, widget, NULL, ei_radiobutton_release, user_param);
    state->radiobutton->selected = state->selected;
    ei_invalidate_rect_from_widget(state->radiobutton);
    free(state);
    return EI_FALSE;
} */

ei_bool_t ei_radiobutton_internal_event(struct ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
    ei_radiobutton_t *radiobutton = (ei_radiobutton_t *)widget;
    ei_radiobutton_set_selected(radiobutton->form, radiobutton->id);
    return EI_FALSE;
}
