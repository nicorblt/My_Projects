#include <assert.h>
#include <string.h>

#include "ei_widgetclass.h"
#include "widgetclasses/ei_frameclass.h"
#include "widgetclasses/ei_buttonclass.h"
#include "widgetclasses/ei_toplevelclass.h"
#include "helpers/ei_memory.h"
#include "internals/ei_widgetclass_internal.h"
#include "ei_event.h"
#include "helpers/ei_string.h"


/**
 * @brief	Registers a class to the program so that widgets of this class can be created.
 *		This must be done only once per widget class in the application.
 *
 * @param	widgetclass	The structure describing the class.
 */
void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
	assert(widgetclass != NULL);
	ei_widgetclass_add(widgetclass);
}

/**
 * @brief	Returns the structure describing a class, from its name.
 *
 * @param	name		The name of the class of widget.
 *
 * @return			The structure describing the class.
 */
ei_widgetclass_t *ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
	for (ei_widgetclass_t *it = ei_widgetclass_get(); it != NULL; it = it->next)
	{
		if (STRCMP(it->name, ==, name))
			return it;
	}
	return NULL;
}

/**
 * \brief	Registers the "frame" widget class in the program. This must be called only
 *		once before widgets of the class "frame" can be created and configured with
 *		\ref ei_frame_configure.
 */
void ei_frame_register_class(void)
{
	ei_widgetclass_t* frameclass = verified_calloc(1, sizeof(ei_widgetclass_t));

	strncpy(frameclass->name, "frame", 6);
	frameclass->allocfunc = ei_frame_allocator;
	frameclass->releasefunc = ei_frame_releaser;
	frameclass->drawfunc = ei_frame_drawer;
	frameclass->setdefaultsfunc = ei_frame_setdefaults;
	frameclass->geomnotifyfunc = ei_frame_geomnotifier;

	ei_widgetclass_register(frameclass);
}

/**
 * \brief	Registers the "button" widget class in the program. This must be called only
 *		once before widgets of the class "button" can be created and configured with
 *		\ref ei_button_configure.
 */
void ei_button_register_class(void)
{
	ei_widgetclass_t* buttonclass = verified_calloc(1, sizeof(ei_widgetclass_t));

	strncpy(buttonclass->name, "button", 7);
	buttonclass->allocfunc = ei_button_allocator;
	buttonclass->releasefunc = ei_button_releaser;
	buttonclass->drawfunc = ei_button_drawer;
	buttonclass->setdefaultsfunc = ei_button_setdefaults;
	buttonclass->geomnotifyfunc = ei_button_geomnotifier;

	ei_widgetclass_register(buttonclass);
	ei_bind(ei_ev_mouse_buttondown, NULL, "button", ei_button_internal_event, NULL);
}

/**
 * \brief	Registers the "toplevel" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void ei_toplevel_register_class(void)
{
	ei_widgetclass_t* toplevelclass = verified_calloc(1, sizeof(ei_widgetclass_t));
	strncpy(toplevelclass->name, "toplevel", 9);
	toplevelclass->allocfunc = ei_toplevel_allocator;
	toplevelclass->releasefunc = ei_toplevel_releaser;
	toplevelclass->drawfunc = ei_toplevel_drawer;
	toplevelclass->setdefaultsfunc = ei_toplevel_setdefaults;
	toplevelclass->geomnotifyfunc = ei_toplevel_geomnotifier;

	ei_widgetclass_register(toplevelclass);
}
