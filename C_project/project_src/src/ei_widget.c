#include <assert.h>

#include "ei_widget.h"
#include "widgets/ei_frame.h"
#include "widgets/ei_toplevel.h"
#include "widgets/ei_button.h"
#include "helpers/ei_memory.h"
#include "ei_geometrymanager.h"
#include "helpers/ei_geometry_helpers.h"
#include "ei_utils.h"
#include "internals/ei_picking_internal.h"
#include "internals/event_internals/event_internal.h"
#include "iterators/ei_widget_tree_iterator.h"
#include "ei_event.h"
#include "ei_application.h"

ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name,
							  ei_widget_t *parent,
							  void *user_data,
							  ei_widget_destructor_t destructor)
{

	// unique id of a widget
	static uint32_t pick_id = 0xff000001;		 // 0 is root
	if (parent == NULL || pick_id == 0xffffffff) // numeric limits for uint32_t
		return NULL;

	ei_widgetclass_t *widgetclass = ei_widgetclass_from_name(class_name);
	if (widgetclass == NULL)
		return NULL;
	// allocates the memory
	ei_widget_t *new_widget = widgetclass->allocfunc();
	if (new_widget == NULL)
		return NULL;

	// Initialize the 'pure' widget part.
	new_widget->wclass = widgetclass;
	new_widget->user_data = user_data;
	new_widget->destructor = destructor;

	// Update hierarchy
	new_widget->parent = parent;
	new_widget->next_sibling = NULL;

	if (parent->children_tail != NULL)
	{
		ei_widget_t *tmp = parent->children_head;
		parent->children_tail->next_sibling = new_widget;
		parent->children_tail = new_widget;
		parent->children_head = tmp;
	}
	else
	{
		parent->children_head = new_widget;
		parent->children_tail = new_widget;
	}
	// pick a unique identifier
	new_widget->pick_id = pick_id++;
	// TODO decommenter ligne suivante pour la vraie application
	new_widget->pick_color = ei_pick_color_from_pick_id(new_widget->pick_id);
	// TODO decommenter cette ligne pour les unitaires
	// new_widget->pick_color = NULL;

	new_widget->geom_params = NULL;
	new_widget->requested_size = ei_size_zero();
	new_widget->screen_location = ei_rect_zero();
	new_widget->content_rect = &new_widget->screen_location;

	// set class specific default values
	widgetclass->setdefaultsfunc(new_widget);

	// Register the widget to the event manager, so that it can be later found by a tag.
	ei_register_widget_to_all_events(new_widget);

	return new_widget;
}

void ei_widget_destroy(ei_widget_t *widget)
{
	if (widget->destructor != NULL)
		widget->destructor(widget);

	ei_geometrymanager_unmap(widget);
	ei_unregister_widget_to_all_events(widget);

	// release the widget part
	// destroys all its descendants
	for (ei_widget_t *it = widget->children_head; it != NULL; it = it->next_sibling)
	{
		ei_widget_destroy(it);
	}
	free(widget->pick_color);
	free(widget->user_data);
	if (widget->content_rect != &widget->screen_location)
		free(widget->content_rect);

	if (widget->wclass->releasefunc != NULL)
		widget->wclass->releasefunc(widget);
	// Make the parent forget it
	if (widget->parent != NULL)
	{
		ei_widget_t sent;
		sent.next_sibling = widget->parent->children_head;
		ei_widget_t *prev = &sent;
		while (prev->next_sibling != NULL)
		{
			if (prev->next_sibling->pick_id == widget->pick_id)
			{
				prev->next_sibling = prev->next_sibling->next_sibling;
				widget->parent->children_head = sent.next_sibling;
				break;
			}
			prev = prev->next_sibling;
		}
	}

	// free the widget itself.
	free(widget);
}

ei_widget_t *ei_widget_pick(ei_point_t *where)
{
	uint32_t pick_id = ei_app_pick_surface_get_color(where);

	ei_iterator_t widget_it = ei_it_begin(widget_tree, ei_app_root_widget());
	ei_widget_t *clicked_on = NULL; //
	for (; ei_it_valid(&widget_it); ei_it_next(&widget_it))
	{
		ei_widget_t *widget = ei_it_get_t(widget_tree, &widget_it);
		if (widget->pick_id == pick_id)
		{
			clicked_on = widget; /* TODO perf peuvent etre améliorés en parcourant la structure des events (on prend un event au hasard puisque un widget est enregistré auprès de tous)
			 puis on regarde les hashmap de tous les tags à l'index #pick_id, on parcoure la liste chainée pour voir si on trouve le widget*/
			break;
		}
	}
	ei_it_destroy(&widget_it);
	return clicked_on;
}

void ei_frame_configure(ei_widget_t *widget,
						ei_size_t *requested_size,
						const ei_color_t *color,
						int *border_width,
						ei_relief_t *relief,
						char **text,
						ei_font_t *text_font,
						ei_color_t *text_color,
						ei_anchor_t *text_anchor,
						ei_surface_t *img,
						ei_rect_t **img_rect,
						ei_anchor_t *img_anchor)
{
	ei_frame_t *frame = (ei_frame_t *)widget;
	ei_bool_t resized_or_moved = EI_FALSE;
	ei_bool_t invalidated = (requested_size || color || border_width || relief || text || text_font ||
							 text_anchor || img || img_rect || img_anchor);
	const ei_size_t initial_requested_size = frame->widget.requested_size;

	/* ****** BEGIN DEFAULTS ****** */
	if (color != NULL)
		frame->color = *color;

	if (border_width != NULL)
	{
		frame->border_width = *border_width;
		resized_or_moved = EI_TRUE;
	}

	if (relief != NULL)
		frame->relief = *relief;

	if (text != NULL)
	{
		// TODO réfléchir sur cette implémentation, peut couter cher pour de très longs textes. Demander conseils à prof car on fait une copie a cahque fois.
		ei_frame_set_text(frame, *text, strlen(*text));
		resized_or_moved = EI_TRUE;
	}

	if (text_font != NULL)
	{
		frame->text_aspect.font = text_font;
		resized_or_moved = EI_TRUE;
	}

	if (text_color != NULL)
		frame->text_aspect.color = *text_color;

	if (text_anchor != NULL)
	{
		frame->text_aspect.anchor = *text_anchor;
		resized_or_moved = EI_TRUE;
	}

	if (img != NULL)
	{
		frame->bg_image = *img;
		resized_or_moved = EI_TRUE;
	}

	if (img_rect != NULL)
	{
		frame->bg_image_aspect.area = *img_rect;
		resized_or_moved = EI_TRUE;
	}

	if (img_anchor != NULL)
	{
		frame->bg_image_aspect.anchor = *img_anchor;
		resized_or_moved = EI_TRUE;
	}

	if (requested_size != NULL)
	{
		frame->widget.requested_size = *requested_size;
	}
	else
	{
		frame->widget.requested_size = ei_size_add(ei_size_zero(), ei_size(frame->border_width, frame->border_width));
#ifndef RELEASE
		assert(!(frame->text != NULL && frame->bg_image != NULL) && "A frame text and bg_image cannot be non-null at the same time.\n");
#endif
		if (frame->text != NULL)
		{
			ei_size_t text_size = {0, 0};
			hw_text_compute_size(frame->text, frame->text_aspect.font, &text_size.width, &text_size.height);
			frame->widget.requested_size = ei_size_add(frame->widget.requested_size, text_size);
		}
		else if (frame->bg_image != NULL)
		{
			ei_size_t img_size = (frame->bg_image_aspect.area != NULL)
									 ? frame->bg_image_aspect.area->size
									 : hw_surface_get_size(frame->bg_image);

			frame->widget.requested_size = ei_size_add(frame->widget.requested_size, img_size);
		}
	}
	/* ****** END DEFAULTS ****** */

	if (EI_COMPARE_SIZE(initial_requested_size, !=, frame->widget.requested_size))
	{
		resized_or_moved = EI_TRUE;
	}

	if (resized_or_moved && frame->widget.geom_params != NULL)
	{
		// update the current widget
		frame->widget.geom_params->manager->runfunc(widget);
	}
	if (invalidated)
	{
		ei_invalidate_rect_from_widget(widget);
	}
}

/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. The only
 *				difference is that relief defaults to \ref ei_relief_raised
 *				and border_width defaults to \ref k_default_button_border_width.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to \ref k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
void ei_button_configure(ei_widget_t *widget,
						 ei_size_t *requested_size,
						 const ei_color_t *color,
						 int *border_width,
						 int *corner_radius,
						 ei_relief_t *relief,
						 char **text,
						 ei_font_t *text_font,
						 ei_color_t *text_color,
						 ei_anchor_t *text_anchor,
						 ei_surface_t *img,
						 ei_rect_t **img_rect,
						 ei_anchor_t *img_anchor,
						 ei_callback_t *callback,
						 void **user_param)
{
	ei_button_t *button = (ei_button_t *)widget;
	void *old_user_param = button->frame.widget.user_data;
	if (user_param != NULL)
		button->frame.widget.user_data = *user_param;
	if (corner_radius != NULL)
		button->corner_radius = *corner_radius;
	if (callback != NULL)
	{
		ei_callback_t old_callback = button->callback;
		void *current_user_param = button->frame.widget.user_data;
		button->callback = *callback;
		ei_unbind(ei_ev_mouse_buttondown, widget, NULL, old_callback, old_user_param);
		ei_bind(ei_ev_mouse_buttondown, widget, NULL, *callback, current_user_param);
	}
	ei_frame_configure(widget, requested_size, color, border_width, relief,
					   text, text_font, text_color, text_anchor, img, img_rect, img_anchor);
}

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title displayed in the title bar. Defaults to "Toplevel".
 *				Uses the font \ref ei_default_font.
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to \ref EI_TRUE.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size of its content.
 *				The default minimum size of a toplevel is (160, 120).
 *				If *min_size is NULL, this requires the min_size to be configured to
 *				the default size.
 */
void ei_toplevel_configure(ei_widget_t *widget,
						   ei_size_t *requested_size,
						   ei_color_t *color,
						   int *border_width,
						   char **title,
						   ei_bool_t *closable,
						   ei_axis_set_t *resizable,
						   ei_size_t **min_size)
{
	ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
	ei_bool_t resized_or_moved = EI_FALSE;
	const ei_size_t initial_requested_size = toplevel->widget.requested_size;
	ei_bool_t invalidated = (requested_size || color || border_width || title || closable || resizable || min_size);

	/* ****** BEGIN DEFAULTS ****** */
	if (color != NULL)
		toplevel->color = *color;

	if (border_width != NULL)
	{
		toplevel->border_width = *border_width;
	}
	if (closable != NULL)
	{
		if (toplevel->closable != *closable)
		{
			if (toplevel->closable) // toplevel is no longer closable, free the close 'button'
			{
				ei_geometrymanager_unmap(widget->children_head);
			}
			else
			{
				ei_anchor_t anc_button = ei_anc_center;
				ei_place(widget->children_head, &anc_button, &toplevel_close_button_x, &toplevel_close_button_y, NULL, NULL, NULL, NULL, NULL, NULL);
			}
		}
		toplevel->closable = *closable;
	}
	if (resizable != NULL) // si resiable, mettre le bouton de redimensionnement toujours en children_tail
	{
		toplevel->axis_set = *resizable;
	}
	if (min_size != NULL)
	{
		toplevel->min_size = **min_size;
		resized_or_moved = EI_TRUE;
	}

	if (title != NULL)
	{
		// TODO réfléchir sur cette implémentation, peut couter cher pour de très longs textes. Demander conseils à prof.
		size_t new_length = strlen(*title) + 1;
		toplevel->title = verified_realloc(toplevel->title, new_length); // add 1 for \0 at the end.
		strncpy(toplevel->title, *title, new_length);
	}

	if (requested_size != NULL)
	{
		toplevel->widget.requested_size = *requested_size;
	}

	if (EI_COMPARE_SIZE(initial_requested_size, !=, toplevel->widget.requested_size))
	{
		resized_or_moved = EI_TRUE;
	}

	if (resized_or_moved && toplevel->widget.geom_params != NULL)
	{
		toplevel->widget.geom_params->manager->runfunc(widget);
	}
	if (invalidated)
	{
		ei_invalidate_rect_from_widget(widget);
	}
}
