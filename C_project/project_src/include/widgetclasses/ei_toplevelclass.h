/**
 *  @file	ei_toplevelclass.h
 *  @brief	Contains base function for the toplevel class
 *  like the allocator, the drawer, the setdefaults, the releaser and the notifier.
 *
 *  @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 *
 *  @version 0.1
 *  @date 2022-15-05
 *
 *  @copyright Copyright (c) 2022
 */

#ifndef EI_TOPLEVELCLASS_H
#define EI_TOPLEVELCLASS_H

#include "ei_types.h"

struct      ei_widget_t;

typedef     void        *ei_surface_t;

/**
 * @brief	A function that releases the memory used by a toplevel before it is destroyed.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void        ei_toplevel_releaser        (struct ei_widget_t *widget);

/**
 * @brief Allocates memory for a toplevel widget.
 *
 * @return ei_widget_t* A pointer to the newly allocated toplevel widget.
 */
struct      ei_widget_t     *ei_toplevel_allocator(void);

/**
 * @brief	Implement the drawer of the toplevel widget.
 *
 * @param	widget		The widget to draw.
 * @param	surface		The surface to draw on.
 * @param   pick_surface	The surface to draw the widget pick_color on.
 * @param	clipper		Define the clipping rectangle to draw only on this rectangle (NULL if no clipping).
 *              Note that the close button is drawn on a clipping zone that take in all the window, including its borders.
 *
 */
void        ei_toplevel_drawer      (struct ei_widget_t *widget, 
                                    ei_surface_t surface, 
                                    ei_surface_t pick_surface, 
                                    ei_rect_t *clipper);

/**
 * @brief	A function that sets the default values for a toplevel.
 *
 * @param	toplevel		A pointer to the toplevel instance to initialize.
 */
void        ei_toplevel_setdefaults     (struct ei_widget_t *toplevel);

/**
 * @brief	Notifies the toplevel that its geometry has been modified by its geometry manager
 * (can be a resize and/or a move).
 * Compute the new size and position of the toplevel's content_rect.
 *
 * @param	widget		The toplevel widget to notify.
 */
void        ei_toplevel_geomnotifier       (struct ei_widget_t *widget);


#endif