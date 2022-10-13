#include <assert.h>

#include "geometry_managers/ei_placer.h"
#include "helpers/ei_string.h"

/**
 * \brief	A function called when a widget cease to be managed by the placer manager.
 *		Most of the work is done in \ref ei_geometrymanager_unmap. This function hook is
 *		only provided to trigger recomputation when the disappearance of a widget has an
 *		effect on the geometry of other widgets.
 *
 * @param	widget		The widget instance that must be forgotten by the placer manager.
 */
void ei_placer_release(struct ei_widget_t *widget)
{
	// FIXME release func de placer à finir (trigger recomputation when the disappearance of a widget has an effect on the geometry of other widgets.)
}

/**
 * \brief	A function that runs the ei_place geometry computation for this widget. This may trigger
 *		geometry computation for this widget's parent and the other children of the parent.
 *
 * @param	widget		The widget instance for which to compute geometry.
 */
void ei_placer_runfunc(struct ei_widget_t *widget)
{
	ei_rect_t old_scr_location = widget->screen_location;
	ei_place(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_placer_setdefault_params(struct ei_widget_t *widget)
{
	assert(widget->geom_params != NULL && STRCMP(widget->geom_params->manager->name, ==, "placer") && "The widget geometry manager is not placer !\n");

	ei_placer_param_t *params = (ei_placer_param_t *)widget->geom_params;

	// POSITION
	params->x = 0;
	params->y = 0;
	params->rel_x = 0.0;
	params->rel_y = 0.0;
	// SIZE
	params->width = widget->requested_size.width;
	params->height = widget->requested_size.height;
	params->rel_width = 0.0;
	params->rel_height = 0.0;
	// ANCHOR
	params->anchor = ei_anc_northwest;
}