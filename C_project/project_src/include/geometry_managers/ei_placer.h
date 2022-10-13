#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "ei_geometrymanager.h"
#include "ei_types.h"

/**
 * @brief	A function called when a widget cease to be managed by the placer manager.
 *		Most of the work is done in \ref ei_geometrymanager_unmap. This function hook is
 *		only provided to trigger recomputation when the disappearance of a widget has an
 *		effect on the geometry of other widgets.
 *
 * @param	widget		The widget instance that must be forgotten by the placer manager.
 */
void        ei_placer_release       (struct ei_widget_t *widget);

/**
 * @brief	A function that runs the ei_place geometry computation for this widget. This may trigger
 *		geometry computation for this widget's parent and the other children of the parent.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
void        ei_placer_runfunc       (struct ei_widget_t *widget);

/**
 * @brief A function that set specific placer geometry parameters to their default values.
 * The widget must already have its geometry manager set to placer, or this will crash.
 *
 * @param widget The widget instance for which to initialize its ei_geom_params's fields.
 */
void        ei_placer_setdefault_params     (struct ei_widget_t *widget);

/**
 * @brief 	A structure that stores information about the geometry manager managing a widget,
 *		and the widget's geometry management parameters. This a the generic type. Each
 *		geometry manager adds field after "manager".
 */
typedef struct ei_placer_param_t
{
    ei_geometry_param_t geom_param; ///< Emulate inheritance by storing a direct ancestor first in the struct. Allows polymorphism too.
    ei_anchor_t anchor;             ///< The anchor of the widget.
    int x;                          ///< The x coordinate of the widget.
    int y;                          ///< The y coordinate of the widget.
    int width;                      ///< The width of the widget.
    int height;                     ///< The height of the widget.
    float rel_x;                    ///< The relative x coordinate of the widget.
    float rel_y;                    ///< The relative y coordinate of the widget.
    float rel_width;                ///< The relative width of the widget.
    float rel_height;               ///< The relative height of the widget.
} ei_placer_param_t;

#endif