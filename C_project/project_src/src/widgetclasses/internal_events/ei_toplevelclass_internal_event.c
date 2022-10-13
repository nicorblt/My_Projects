#include "widgetclasses/internal_events/ei_toplevelclass_internal_event.h"

#include "ei_event.h"
#include "ei_utils.h"
#include "widgetclasses/config/ei_toplevelclass_config.h"
#include "widgets/ei_toplevel.h"
#include "helpers/ei_memory.h"
#include "internals/ei_application_internal.h"

/*< HELPERS FUNCTIONS >*/
// we know that the click is on the toplevel
static ei_bool_t point_is_in_strip_band(ei_widget_t *widget, ei_point_t *where)
{
    ei_point_t relative_pos = ei_point_sub(*where, widget->screen_location.top_left);
    if (relative_pos.y < widget->screen_location.top_left.y + k_toplevel_strip_height)
        return EI_TRUE;
    return EI_FALSE;
}

static ei_bool_t point_is_on_resize_square(ei_widget_t *widget, ei_point_t *where)
{
    ei_point_t toplevel_bot_right = ei_point_add(widget->screen_location.top_left, ei_size_as_point(widget->screen_location.size));
    ei_rect_t resize_square = {
        {toplevel_bot_right.x - k_bottom_right_rectangle_width, toplevel_bot_right.y - k_bottom_right_rectangle_width}, \
        {k_bottom_right_rectangle_width, k_bottom_right_rectangle_width}       
        };
    return ei_point_is_in_rect(*where, resize_square);
}


/*< USER PARAM STRUCTURES >*/

struct move_user_param
{
    ei_widget_t *previous_tail;
    ei_geometry_param_t *geom_params;
};

struct resize_user_param
{

};


/*< MOVE INTERNAL >*/
static ei_bool_t ei_toplevel_move_until_mouse_release(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    struct move_user_param *param = (struct move_user_param *) user_param;
    ei_invalidate_rect_from_widget(widget);
    ei_point_t parent_pos = ei_point_zero();
    if (widget->parent != NULL)
        parent_pos = widget->parent->content_rect->top_left;
    ei_point_t offset = ei_point_sub(event->param.mouse.where, parent_pos);
    ei_place(widget, NULL, offset.x, offset.y, NULL, NULL, NULL, NULL, NULL, NULL);
    return EI_TRUE;
}


static ei_bool_t ei_toplevel_move_mouse_release(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    // FIXME remettre les enfants de toplevel parent dans le sens initial
    widget->geom_params->manager->releasefunc(widget);
    widget->geom_params = ((struct move_user_param *)user_param)->geom_params;
    free((struct move_user_param *)user_param);
    ei_unbind(ei_ev_mouse_buttonup, widget, NULL, ei_toplevel_move_mouse_release, user_param);
    ei_unbind(ei_ev_mouse_move, widget, NULL, ei_toplevel_move_until_mouse_release, user_param);
    return EI_FALSE;
}


ei_bool_t ei_toplevel_click_internal_event(struct ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
    ei_toplevel_t * toplevel = (ei_toplevel_t *)widget;
    ei_point_t mouse_pos = event->param.mouse.where;
    if (point_is_in_strip_band(widget, &mouse_pos)) // check si pas sur le bouton qui quitte
    {
        // if click on the strip band, toplevel must go on the foreground
        // FIXME mettre toplevel en children tail
        struct move_user_param *user_param = verified_calloc(1, sizeof(struct move_user_param));
        user_param->geom_params = widget->geom_params;
        user_param->previous_tail = NULL;
        ei_point_t parent_pos = ei_point_zero();
        if (widget->parent != NULL)
            parent_pos = widget->parent->content_rect->top_left;
        ei_point_t offset = ei_point_sub(mouse_pos, parent_pos);
        ei_place(widget, NULL, offset.x, offset.y, NULL, NULL, NULL, NULL, NULL, NULL);

        ei_bind(ei_ev_mouse_buttonup, widget, NULL, ei_toplevel_move_mouse_release, NULL);
        ei_bind(ei_ev_mouse_move, widget, NULL, ei_toplevel_move_until_mouse_release, &user_param);
    }
    else if (toplevel->axis_set != ei_axis_none && point_is_on_resize_square(widget, &mouse_pos))
    {
        
    }
    return EI_FALSE;
}