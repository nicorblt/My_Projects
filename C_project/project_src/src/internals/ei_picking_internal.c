/**
 * @brief   Defines the pick_surface of type \ref ei_surface_t.
 * The purpose of the pick_surface is to draw a surface under buttons to be able to determine which button has been clicked.
 */

#include "internals/ei_picking_internal.h"
#include "helpers/ei_draw_helpers.h"
#include "helpers/ei_memory.h"
#include "hw_interface.h"
#include "ei_types.h"

static ei_surface_t pick_surface = NULL;

void ei_app_pick_surface_create(ei_surface_t root_surface)
{
    if (pick_surface == NULL)
    {
        ei_size_t root_size = hw_surface_get_size(root_surface);
        pick_surface = hw_surface_create(root_surface, root_size, EI_TRUE);
    }
}

ei_surface_t ei_app_pick_surface(void)
{
    return pick_surface;
}

void ei_app_pick_surface_destroy()
{
    hw_surface_free(pick_surface);
    pick_surface = NULL;
}

uint32_t ei_app_pick_surface_get_color(ei_point_t *where)
{
    ei_rect_t clipper = {{where->x, where->y}, {1, 1}};
    uint32_t pixel = *offset_buffer_from_clipped(pick_surface, &clipper);
    return pixel;
}


ei_color_t *ei_pick_color_from_pick_id(uint32_t pick_id)
{
    ei_color_t *color = (ei_color_t *)verified_calloc(1, sizeof(ei_color_t));
    int ir, ig, ib, ia;
    ir = ig = ib = ia = 0;
    hw_surface_get_channel_indices(ei_app_pick_surface(), &ir, &ig, &ib, &ia);
    if (ia == -1)
    {
        ia = (ia == -1 && ((ib != 3) && (ig != 3) && (ir != 3))) ? 3 : ia;
        ia = (ia == -1 && ((ib != 2) && (ig != 2) && (ir != 2))) ? 2 : ia;
        ia = (ia == -1 && ((ib != 1) && (ig != 1) && (ir != 1))) ? 1 : ia;
        ia = (ia == -1 && ((ib != 0) && (ig != 0) && (ir != 0))) ? 0 : ia;
    }
    color->red = (unsigned char)(pick_id >> (ir * 8) & 255);
    color->green = (unsigned char)(pick_id >> (ig * 8) & 255);
    color->blue = (unsigned char)(pick_id >> (ib * 8) & 255);
    color->alpha = (unsigned char)(pick_id >> (ia * 8) & 255);
    return color;
}
