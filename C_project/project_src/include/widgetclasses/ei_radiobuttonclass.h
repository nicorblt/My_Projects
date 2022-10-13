/**
 *  @file	ei_radiobuttonclass.h
 *  @brief	Contains base function for the radiobutton class
 *  like the allocator, the drawer, the setdefaults, the releaser and the notifier.
 *
 *  @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 *
 *  @version 0.1
 *  @date 2022-15-05
 *
 *  @copyright Copyright (c) 2022
 */


#ifndef EI_RADIOBUTTONCLASS_H
#define EI_RADIOBUTTONCLASS_H

#include "ei_types.h"

struct      ei_widget_t;
struct      ei_linked_radiobutton_t;

typedef     void        *ei_surface_t;

/**
 * \brief	Registers the "radiobutton" widget class in the program. This must be called only
 *		once before widgets of the class "radiobutton" can be created and configured with
 *		\ref ei_radiobutton_configure.
 */
void        ei_radiobutton_register_class       (void);

/**
 *  @brief    Allocates memory for a new frame widget.
 *
 *  @return   ei_widget_t *
 *
 */
struct      ei_widget_t      *ei_radiobutton_allocator(void);

/**
 *  @brief
 * A function that releases the memory used by a radiobutton before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* by freed by
 *		these functions. They free the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *  @param   widget   The radiobutton which resources are to be freed.
 *
 */
void        ei_radiobutton_releaser      (struct ei_widget_t *widget);

/**
 *  @brief	Implement the drawer of the radiobutton widget.
 *
 *  @param	widget		    The radiobutton to draw.
 *  @param	surface		    The surface to draw on.
 *  @param  pick_surface	The surface to draw the pick color on.
 *  @param	clipper		    Define the clipping rectangle to draw only on this rectangle (NULL if no clipping).
 *
 */
void        ei_radiobutton_drawer        (struct ei_widget_t *widget, 
                                         ei_surface_t surface, 
                                         ei_surface_t pick_surface, 
                                         ei_rect_t *clipper);

/**
 *  @brief	Implement the drawer of the radiobutton widget.
 *
 *  @param	linked_list		The radiobutton linked list to draw in the order of the linked list.
 *  @param	surface		    The surface to draw on.
 *  @param	clipper		    Define the clipping rectangle to draw only on this rectangle (NULL if no clipping).
 *
 */
void        ei_linked_radiobutton_drawer        (struct ei_linked_radiobutton_t *linked_list,
                                                ei_surface_t surface, 
                                                ei_rect_t *clipper);

/**
 *  @brief	A function that sets the default values for a radiobutton.
 *
 *  @param	radiobutton		A pointer to the radiobutton instance to initialize.
 */
void        ei_radiobutton_setdefaults       (struct ei_widget_t *radio);

/**
 * @brief	Notifies the radiobutton that its geometry has been modified by its geometry manager
 * (can be a resize and/or a move).
 * Compute the new size and position of the widget's content_rect.
 *
 * @param	widget		The widget radiobutton to notify.
 */
void        ei_radiobutton_geomnotifier       (struct ei_widget_t *widget);

ei_bool_t ei_radiobutton_internal_event(struct ei_widget_t *widget, struct ei_event_t *event, void *user_param);

#endif