#include "widgets/ei_radiobutton.h"
#include "ei_widget.h"
#include "iterators/ei_iterator.h"
#include "helpers/ei_memory.h"
#include "helpers/ei_string.h"
#include "ei_event.h"

void ei_radiobutton_configure(ei_widget_t *widget,
                              ei_size_t *requested_size,
                              const ei_color_t *color,
                              const ei_color_t *fill_color,
                              int *border_width,
                              ei_relief_t *relief,
                              ei_type_shape_t *shape,
                              ei_bool_t *is_selected,
                              char **text,
                              ei_font_t *text_font,
                              ei_color_t *text_color,
                              ei_anchor_t *text_anchor,
                              ei_callback_t *callback,
                              void **user_param)
{
    ei_radiobutton_t *radio = (ei_radiobutton_t *)widget;
    void *old_user_param = radio->frame.widget.user_data;

    if (user_param != NULL)
        radio->frame.widget.user_data = *user_param;
    if (shape != NULL)
        radio->shape.type = *shape;
    if (fill_color != NULL)
        radio->fill_color = *fill_color;

    if (is_selected != NULL)
        radio->selected = *is_selected;
    else
        radio->selected = EI_FALSE;

    if (callback != NULL)
    {
        ei_callback_t old_callback = radio->callback;
        void *current_user_param = radio->frame.widget.user_data;
        radio->callback = *callback;
        ei_unbind(ei_ev_mouse_buttondown, widget, NULL, old_callback, old_user_param);
        ei_bind(ei_ev_mouse_buttondown, widget, NULL, *callback, current_user_param);
    }

    // Disjonction de cas sur la shape
    if (requested_size != NULL)
    {
        switch (radio->shape.type)
        {
        case ei_shape_circle:
            radio->shape.properties.circle.radius = requested_size->width / 2;
            break;
        case ei_shape_square:
            radio->shape.properties.square.size = requested_size->width;
            break;
        case ei_shape_rectangle:
            radio->shape.properties.rectangle.width = requested_size->width;
            radio->shape.properties.rectangle.height = requested_size->height;
            break;
        case ei_shape_diamond:
            radio->shape.properties.diamond.size = requested_size->width;
            break;
        case ei_shape_triangle:
            radio->shape.properties.triangle.base = requested_size->width;
            radio->shape.properties.triangle.height = requested_size->height;
            break;
        default:
            break;
        }
    }

    ei_frame_configure(widget, requested_size, color, border_width, relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_radiobutton_form_append(ei_radiobutton_form_t **form,
                                ei_radiobutton_t *to_append)
{
    if (*form == NULL)
    {
        *form = verified_calloc(1, sizeof(ei_radiobutton_form_t));
        to_append->next = NULL;
        to_append->selected = EI_TRUE; // The default selected radiobutton is the first one of the form
        (*form)->head = to_append;
        (*form)->tail = to_append;
        to_append->id = 0;
        to_append->form = *form;
        return;
    }
    to_append->form = *form;
    to_append->id = (*form)->tail->id + 1;
    (*form)->tail->next = to_append;
    (*form)->tail = to_append;
    to_append->next = NULL;
}

void ei_radiobutton_set_selected(ei_radiobutton_form_t *form, int id)
{
    if (form == NULL || form->tail == NULL)
        return;
    if (id > form->tail->id)
        return;

    ei_radiobutton_t *current = form->head;

    while (current != NULL)
    {
        if (current->id == id)
        {
            current->selected = EI_TRUE;
            ei_invalidate_rect_from_widget(current);
        }
        else if (current->selected == EI_TRUE)
        {
            current->selected = EI_FALSE;
            ei_invalidate_rect_from_widget(current);
        }
        else
        {
            current->selected = EI_FALSE;
        }
        current = current->next;
    }

}
