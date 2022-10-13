/**
 *  @file   test_ei_draw.c
 *  @brief Internal functions for picking.
 *  
 *  @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 *  @version 0.1
 *  @date 2022-05-05
 *  @copyright Copyright (c) 2022
 *
 */

#ifndef EI_PICKING_INTERNAL_H
#define EI_PICKING_INTERNAL_H

#include "ei_types.h"

typedef void * ei_surface_t;

/**
 * @brief Returns the surface of the picking surface.
 *
 */
ei_surface_t ei_app_pick_surface(void);

/**
 * @brief   Create the picking surface off the root surface.
 * This function can only be called once between each ei_app_pick_surface_destroy call.
 * 
 * @param   root_surface    The surface of the root widget, matching the whole window surface.
 */
void ei_app_pick_surface_create(ei_surface_t root_surface);

/**
 * @brief   Free the picking surface and set it to NULL.
 * 
 */
void ei_app_pick_surface_destroy();

/**
 * @brief	Return an \ref ei_color_t from an id.
 *
 * @param	id   The id of the color.
 *
 * @return	The \ref ei_color_t corresponding to the id.
 */
ei_color_t *ei_pick_color_from_pick_id(uint32_t pick_id);


/**
 * @brief	Return the id from color of pick_surface from the point passed in parameter.
 *
 * @param	where   The point than we want to know the id color on the pick_surface
 *
 * @return	The \ref ei_color_t corresponding to the point on pick_surface.
 */
uint32_t  ei_app_pick_surface_get_color(ei_point_t *where);

#endif