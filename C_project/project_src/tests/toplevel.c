#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_code == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
	
	return EI_FALSE;
}


/*
 * main --
 *
 *	Main function of the application.
 */
int main()
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	toplevel1 = NULL;
	ei_size_t		toplevel1_size				= {400, 300};
	int				toplevel1_x					= 50;
	int				toplevel1_y					= 40;
	ei_color_t		toplevel1_color				= {0x55, 0x00, 0x55, 0xff};
	char*			toplevel1_title				= "Mon premier toplevel !";
	int				toplevel1_border_width		= 4;
    ei_axis_set_t 	toplevel1_resizable 		= ei_axis_both;
    ei_bool_t 		toplevel1_closable			= EI_TRUE;
    ei_size_t 		toplevel1_min_size 			= {100,100};
    ei_size_t 		*ptr_toplevel1_min_size 	= &toplevel1_min_size;
	
	ei_widget_t*	frame2 = NULL;
	ei_size_t		frame2_size				= {100, 100};
	int				frame2_x				= 0;
	int				frame2_y				= 0;
	ei_color_t		frame2_color			= {0x88, 0x88, 0x88, 0xff};


	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the button on screen. */
	toplevel1 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure	(toplevel1, NULL, &toplevel1_color,
				 &toplevel1_border_width, &toplevel1_title, &toplevel1_closable, &toplevel1_resizable, &ptr_toplevel1_min_size);
	ei_place(toplevel1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );

	frame2 = ei_widget_create("frame", toplevel1, NULL, NULL);
	ei_frame_configure(frame2, &frame2_size, &frame2_color,
			    NULL, NULL, NULL, NULL, NULL, NULL,
			    NULL, NULL, NULL);
	ei_place(frame2, NULL, &frame2_x, &frame2_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Hook the keypress callback to the event. */
	//ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL); 

	/* Run the application's main loop. */
	ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);


	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	//ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
	ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
	ei_app_free();

	return (EXIT_SUCCESS);
}
