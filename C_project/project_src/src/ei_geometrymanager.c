#include <assert.h>

#include "ei_geometrymanager.h"
#include "ei_types.h"
#include "ei_utils.h"
#include "ei_application.h"
#include "helpers/ei_string.h"
#include "helpers/ei_memory.h"

#include "geometry_managers/ei_placer.h"
#include "internals/ei_geometrymanager_internal.h"
#include "helpers/ei_geometry_helpers.h"

/**
 * \brief	Registers a geometry manager to the program so that it can be called to manager
 *		widgets. This must be done only once in the application.
 *
 * @param	geometrymanager		The structure describing the geometry manager.
 */
void ei_geometrymanager_register(ei_geometrymanager_t *geometrymanager)
{
	assert(geometrymanager != NULL && "Tried to register a NULL geometry manager\n");
	ei_geometrymanager_add(geometrymanager);
}

/**
 * \brief	Returns a geometry manager structure from its name.
 *
 * @param	name		The name of the geometry manager.
 *
 * @return			The structure describing the geometry manager.
 */
ei_geometrymanager_t *ei_geometrymanager_from_name(ei_geometrymanager_name_t name)
{
	// TODO définir un iterateur pour geometrymanager
	for (ei_geometrymanager_t *it = ei_geometrymanager_get(); it != NULL; it = it->next)
	{
		if (STRCMP(it->name, ==, name))
		{
			return it;
		}
	}
	return NULL;
}

/**
 * \brief	Tell the geometry manager in charge of a widget to forget it. This removes the
 *		widget from the screen. If the widget is not currently managed, this function
 *		returns silently.
 *		Side effects:
 *		<ul>
 *			<li> the \ref ei_geometrymanager_releasefunc_t of the geometry manager in
 *				charge of this widget is called, </li>
 *			<li> the geom_param field of the widget is freed, </li>
 *			<li> the current screen_location of the widget is invalided (which will
 *				trigger a redraw), </li>
 *			<li> the screen_location of the widget is reset to 0. </li>
 *		</ul>
 *
 * @param	widget		The widget to unmap from the screen.
 */
void ei_geometrymanager_unmap(ei_widget_t *widget)
{
	// Is widget currently managed ?
	if (widget->geom_params != NULL)
	{
		ei_app_invalidate_rect(&widget->screen_location);
		widget->geom_params->manager->releasefunc(widget);
		widget->screen_location = ei_rect_zero();
		free(widget->geom_params); // FIXME peut etre ecrire une fonction qui fait appel a un liberateur pour geom params (au cas ou un geo manager aurait des ptrs a liberer dans ses params)
	}
}

/**
 * \brief	Registers the "placer" geometry manager in the program. This must be called only
 *		once before the \ref ei_place function can be called.
 */
void ei_register_placer_manager(void)
{
	ei_geometrymanager_t *placer_manager = verified_calloc(1, sizeof(ei_geometrymanager_t));
	strncpy(placer_manager->name, "placer", 7);
	placer_manager->runfunc = ei_placer_runfunc;
	placer_manager->releasefunc = ei_placer_release;
	placer_manager->next = NULL;
	ei_geometrymanager_register(placer_manager);
}

/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 * 		If the widget was already managed by another geometry manager, then it is first
 *		removed from the previous geometry manager.
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then the requested size of the widget is used, i.e. the minimal size
 *		required to display its content.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0) in the
 *				parent's reference.
 * @param	y		The absolute y position of the widget (defaults to 0) in the
 *				parent's reference.
 * @param	width		The absolute width for the widget (defaults to the requested width
 *				of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height
 *				of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the parent, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the parent, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the parent (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the parent (defaults to 0.0).
 */
void ei_place(ei_widget_t *widget,
			  ei_anchor_t *anchor,
			  int *x,
			  int *y,
			  int *width,
			  int *height,
			  float *rel_x,
			  float *rel_y,
			  float *rel_width,
			  float *rel_height)
{
	if (widget->geom_params == NULL)
	{
		widget->geom_params = (ei_geometry_param_t *)verified_calloc(1, sizeof(ei_placer_param_t));
		widget->geom_params->manager = ei_geometrymanager_from_name("placer");
		ei_placer_setdefault_params(widget);
	}
	else if (STRCMP(widget->geom_params->manager->name, !=, "placer"))
	{
		widget->geom_params->manager->releasefunc(widget);
		widget->geom_params = (ei_geometry_param_t *)verified_calloc(1, sizeof(ei_placer_param_t));
		widget->geom_params->manager = ei_geometrymanager_from_name("placer");
		ei_placer_setdefault_params(widget);
	}

	ei_placer_param_t *geom_params = (ei_placer_param_t *)widget->geom_params;
	ei_rect_t old_scr_location = widget->screen_location;

	ei_rect_t parent_content_rect = *widget->parent->content_rect;
	int parent_width = parent_content_rect.size.width;
	int parent_height = parent_content_rect.size.height;

	// we retrieve the height and width of the widget

	/* ******* BEGIN DEFAULTS ******* */

	int screen_location_width = 0;
	int screen_location_height = 0;

	// POSITION
	if (x != NULL)
		geom_params->x = *x;
	if (y != NULL)
		geom_params->y = *y;
	if (rel_x != NULL)
		geom_params->rel_x = *rel_x;
	if (rel_y != NULL)
		geom_params->rel_y = *rel_y;

	// SIZE
	if (rel_width != NULL)
		geom_params->rel_width = *rel_width;
	if (rel_height != NULL)
		geom_params->rel_height = *rel_height;

	if (width == NULL)
	{
		if (rel_width == NULL)
		{
			geom_params->width = widget->requested_size.width;
			screen_location_width = widget->requested_size.width;
		}
		else
		{
			geom_params->width = 0;
			screen_location_width = (*rel_width) * widget->parent->content_rect->size.width;
		}
	}
	else
	{
		geom_params->width = *width;
		screen_location_width = *width;
	}

	if (height == NULL)
	{
		if (rel_height == NULL)
		{
			geom_params->height = widget->requested_size.height;
			screen_location_height = widget->requested_size.height;
		}
		else
		{
			geom_params->height = 0;
			screen_location_height = geom_params->rel_height * widget->parent->content_rect->size.height;
		}
	}
	else
	{
		geom_params->height = *height;
		screen_location_height = *height;
	}
	// ANCHOR
	if (anchor != NULL)
		geom_params->anchor = *anchor;

	/* ******* END DEFAULTS ******* */

	// we retrieve the coordinate of top left point final
	int anchor_x = geom_params->x + (int)(geom_params->rel_x * parent_width);
	int anchor_y = geom_params->y + (int)(geom_params->rel_y * parent_height);
	int x_rel_topleft, y_rel_topleft;

	switch (geom_params->anchor)
	{
	case ei_anc_center:
		x_rel_topleft = anchor_x - (int)geom_params->width / 2;
		y_rel_topleft = anchor_y - (int)geom_params->height / 2;
		break;

	case ei_anc_north:
		x_rel_topleft = anchor_x - (int)geom_params->width / 2;
		y_rel_topleft = anchor_y;
		break;

	case ei_anc_northeast:
		x_rel_topleft = anchor_x - geom_params->width;
		y_rel_topleft = anchor_y;
		break;

	case ei_anc_east:
		x_rel_topleft = anchor_x - geom_params->width;
		y_rel_topleft = anchor_y - (int)geom_params->height / 2;
		break;

	case ei_anc_southeast:
		x_rel_topleft = anchor_x - geom_params->width;
		y_rel_topleft = anchor_y - geom_params->height;
		break;

	case ei_anc_south:
		x_rel_topleft = anchor_x - (int)geom_params->width / 2;
		y_rel_topleft = anchor_y - geom_params->height;
		break;

	case ei_anc_southwest:
		x_rel_topleft = anchor_x;
		y_rel_topleft = anchor_y - geom_params->height;
		break;

	case ei_anc_west:
		x_rel_topleft = anchor_x;
		y_rel_topleft = anchor_y - (int)geom_params->height / 2;
		break;

	case ei_anc_northwest:
		x_rel_topleft = anchor_x;
		y_rel_topleft = anchor_y;
		break;
	default:
		x_rel_topleft = anchor_x;
		y_rel_topleft = anchor_y;
		break;
	}

	ei_point_t parent_topleft = parent_content_rect.top_left;
	widget->screen_location.top_left.x = x_rel_topleft + parent_topleft.x;
	widget->screen_location.top_left.y = y_rel_topleft + parent_topleft.y;

	widget->screen_location.size = ei_size(screen_location_width, screen_location_height);

	if (EI_COMPARE_RECT(old_scr_location, !=, widget->screen_location))
	{
		widget->wclass->geomnotifyfunc(widget);
		ei_widget_t *current = widget->children_head;
		while (current != NULL)
		{
			if (current->geom_params != NULL)
			{
				// Update the geometry of the child by calling geometry manager
				current->geom_params->manager->runfunc(current);
			}
			current = current->next_sibling;
		}
	}
}