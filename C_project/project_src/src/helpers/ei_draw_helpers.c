#include "helpers/ei_draw_helpers.h"
#include "hw_interface.h"
#include "helpers/ei_math.h"


/**
 * @brief Darken or lightnen a rgba color by a certain percentage.
 *
 * @param color The color to work on.
 * @param percentage A value between -1 and 1. Negative darken, positive lighten.
 * @return ei_color_t The resulting color.
 */
ei_color_t modify_rgba_lightness(ei_color_t color, float percentage)
{
    if (percentage < -1.0 || percentage > 1.0)
        return color;

    ei_color_t result;
    float coeff = 1 + percentage;
    if (percentage > 0){
        result.red = MIN(255, (int)( color.red * coeff));
        result.green = MIN(255, (int)( color.green * coeff));
        result.blue = MIN(255, (int)( color.blue * coeff));
    }
    else
    {
        result.red = MAX(0, (int)( 25 + color.red * coeff));
        result.green = MAX(0, (int)( 25 + color.green * coeff));
        result.blue = MAX(0, (int)( 25 + color.blue * coeff));
    }
    result.alpha = color.alpha;
    return result;
}

/**
 * @brief Compute and return the first pixel of a surface within its clipper boundaries. Returns NULL if failure.
 *
 * @param surface The surface to return a pixel of.
 * @param area The subpart of the surface which is within its clipper boundaries.
 *
 * @return uint8_t* The first pixel of the surface which is within its clipper boundaries.
 */
uint32_t *offset_buffer_from_clipped(const ei_surface_t surface, const ei_rect_t *area)
{
    uint32_t *pix = (uint32_t *)hw_surface_get_buffer(surface);
    ei_rect_t surface_rect = hw_surface_get_rect(surface);

    long int x_offset = area->top_left.x - surface_rect.top_left.x;
    if (x_offset < 0)
        return NULL;

    long int y_offset = area->top_left.y - surface_rect.top_left.y;
    if (y_offset < 0)
        return NULL;

    pix += (size_t)x_offset;
    pix += (size_t)y_offset * surface_rect.size.width;
    return pix;
}
