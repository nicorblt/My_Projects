#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"



/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 */
typedef struct ei_toplevel_t
{
    ei_widget_t widget;

    ei_color_t color; ///< The background color of a toplevel.
    int border_width; ///< Width of the border of the toplevel. Can be 0.

    ei_bool_t closable;     ///< Specifies if the window can be closed or not, i.e. if a close button is displayed (if True) or not (if False) on the header bar.
    char *title;            ///< The title of the toplevel which will be displayed on the header bar.
    ei_font_t title_font;   ///< Describes the aspect "title", i.e. its style, font, color and anchor.
    ei_axis_set_t axis_set; ///< Specifies is the widget can be resized or not and if yes on which axes (horizontally and/or vertically).
    ei_size_t min_size;     ///< The minimal size of the window.
} ei_toplevel_t;

static const int             k_toplevel_strip_height = 35 ;      ///< The height of the head stip of toplevel
static int		        toplevel_close_button_x		            = (int)(5*(k_toplevel_strip_height >> 3));
static int		        toplevel_close_button_y		            = (int)(k_toplevel_strip_height >> 1);


/* ***** callbacks ***** */
ei_bool_t ei_toplevel_close_button_internal_quit_event(ei_widget_t* widget, ei_event_t* event, void* user_param);

#endif