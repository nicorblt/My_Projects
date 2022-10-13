/**
 *  @file	ei_buttonclass.h
 *  @brief	Contains base function for the button class
 *  like the allocator, the drawer, the setdefaults, the releaser and the notifier.
 *
 *  @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 *
 *  @version 0.1
 *  @date 2022-15-05
 *
 *  @copyright Copyright (c) 2022
 */


#ifndef EI_BUTTONCLASS_H
#define EI_BUTTONCLASS_H

#include "ei_types.h"

struct      ei_widget_t;
struct ei_event_t;

typedef     void        *ei_surface_t;

/**
 *  @brief    Allocates memory for a new frame widget.
 *
 *  @return   ei_widget_t *
 *
 */
struct      ei_widget_t      *ei_button_allocator(void);

/**
 *  @brief
 * A function that releases the memory used by a button before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* by freed by
 *		these functions. They free the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *  @param   widget   The button which resources are to be freed.
 *
 */
void        ei_button_releaser      (struct ei_widget_t *widget);

/**
 *  @brief	Implement the drawer of the button widget.
 *
 *  @param	widget		The widget to draw.
 *  @param	surface		The surface to draw on.
 *  @param  pick_surface	The surface to draw the pick color on.
 *  @param	clipper		Define the clipping rectangle to draw only on this rectangle (NULL if no clipping).
 *              Note that the close button is drawn on a clipping zone that take in all the window, including its borders.
 *
 */
void        ei_button_drawer        (struct ei_widget_t *widget, 
                                    ei_surface_t surface, 
                                    ei_surface_t pick_surface, 
                                    ei_rect_t *clipper);

/**
 *  @brief	A function that sets the default values for a button.
 *
 *  @param	button		A pointer to the button instance to initialize.
 */
void        ei_button_setdefaults       (struct ei_widget_t *button);

/**
 * @brief	Notifies the button that its geometry has been modified by its geometry manager
 * (can be a resize and/or a move).
 * Compute the new size and position of the widget's content_rect.
 *
 * @param	widget		The widget button to notify.
 */
void        ei_button_geomnotifier       (struct ei_widget_t *widget);


ei_bool_t ei_button_internal_event(struct ei_widget_t *widget, struct ei_event_t *event, void *user_param);
#endif