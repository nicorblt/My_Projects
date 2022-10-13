#include <stdio.h>

#include "widgets/ei_toplevel.h"
#include "widgetclasses/ei_toplevelclass.h"

ei_bool_t ei_toplevel_close_button_internal_quit_event(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    ei_widget_destroy(widget->parent);
   // ei_toplevel_releaser(widget);
    printf("Au revoir ! \n");
	return EI_TRUE;
}
