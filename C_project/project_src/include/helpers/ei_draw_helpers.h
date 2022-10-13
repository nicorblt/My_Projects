/**
 *  @file	ei_draw_helpers.h
 *  @brief  Contains base helpers for the drawing functions.
 *  For example, the functions that modify the lightness of a widget or that gives the first pixel inside boundaries of a surface.
 *
 *  @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 *
 *  @version 0.1
 *  @date 2022-15-05
 *
 *  @copyright Copyright (c) 2022
 */

#ifndef EI_DRAW_HELPERS_H
#define EI_DRAW_HELPERS_H

#include "ei_types.h"

typedef     void        *ei_surface_t;

/**
 * @brief Darken or lightnen a rgba color by a certain percentage.
 *
 * @param color The color to work on.
 * @param percentage A value between -1 and 1. Negative darken, positive lighten.
 * @return ei_color_t The resulting color.
 */
ei_color_t      modify_rgba_lightness       (ei_color_t color, float percentage);

/**
 * @brief Compute and return the first pixel of a surface within its clipper boundaries. Returns NULL if failure.
 *
 * @param surface The surface to return a pixel of.
 * @param area The subpart of the surface which is within its clipper boundaries.
 *
 * @return uint8_t* The first pixel of the surface which is within its clipper boundaries.
 */
uint32_t        *offset_buffer_from_clipped     (const ei_surface_t surface,
                                                const ei_rect_t *area);

#endif