#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
	return EI_TRUE;
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
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


	ei_app_create(screen_size, EI_FALSE);


	ei_widget_t*	button1;
	ei_size_t		button1_size		= {200,300};
	int				button1_x			= 150;
	int				button1_y			= 200;
	ei_color_t		button1_color		= {0x00, 0x00, 0x00, 0xff};

    ei_surface_t 	img1 			= hw_image_load("misc/klimt.jpg", ei_app_root_surface());
    ei_point_t		point_img1 	= {250,0};
    ei_size_t 		size_img1 		= {100,400};
    ei_rect_t 		img1_rect  		= {point_img1, size_img1};
    ei_rect_t 		*ptr_img1_rect 	= &img1_rect;
    ei_anchor_t 	img1_anchor		= ei_anc_northwest;

	int				button1_corner_radius	= 20;
	ei_relief_t		button1_relief	= ei_relief_raised;
	int				button1_border_width	= 6;
	ei_callback_t	button1_callback = button_press;


	ei_widget_t*	button2;
	ei_size_t		button2_size		= {500,200};
	int				button2_x			= 400;
	int				button2_y			= 10;
	ei_color_t		button2_color		= {0xff, 0xff, 0xff, 0xff};

	char*			button2_title		= "Ceci est un texte trop long pour un bouton";
	ei_color_t		button2_text_color	= {0x00, 0x00, 0x00, 0xff};
	ei_anchor_t		button2_text_anchor	= ei_anc_center;

	int				button2_corner_radius	= 30;
	ei_relief_t		button2_relief		= ei_relief_raised;
	int				button2_border_width	= 6;
	ei_callback_t	button2_callback = button_press;


	ei_widget_t*	button3;
	// ei_size_t		button3_size		= {200,300};
	int				button3_x			= 400;
	int				button3_y			= 400;
	ei_color_t		button3_color		= {0x40, 0xb2, 0xa0, 0xff};

    ei_surface_t 	img3 			= hw_image_load("misc/quadrant.png", ei_app_root_surface());
    ei_point_t		point_img3 	= {0,0};
    ei_size_t 		size_img3 		= {100,100};
    ei_rect_t 		img3_rect  		= {point_img3, size_img3};
    ei_rect_t 		*ptr_img3_rect 	= &img3_rect;
    ei_anchor_t 	img3_anchor		= ei_anc_northwest;

	int				button3_corner_radius	= 15;
	ei_relief_t		button3_relief	= ei_relief_raised;
	int				button3_border_width	= 6;
	ei_callback_t	button3_callback = button_press;


	/* Create the application and change the color of the background. */
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the button on screen. */
	button1 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
	ei_button_configure	(button1, &button1_size, &button1_color,
				 &button1_border_width, &button1_corner_radius, &button1_relief,
                 NULL, NULL, NULL, NULL,
				 &img1, &ptr_img1_rect, &img1_anchor,
                 &button1_callback, NULL);
	ei_place(button1, NULL, &button1_x, &button1_y, NULL, NULL, NULL, NULL, NULL, NULL );


	button2 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
	ei_button_configure	(button2, &button2_size, &button2_color,
				 &button2_border_width, &button2_corner_radius, &button2_relief,
                 &button2_title, NULL, &button2_text_color, &button2_text_anchor,
				 NULL, NULL, NULL,
                 &button2_callback, NULL);
	ei_place(button2, NULL, &button2_x, &button2_y, NULL, NULL, NULL, NULL, NULL, NULL );


	button3 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
	ei_button_configure	(button3, NULL, &button3_color,
				 &button3_border_width, &button3_corner_radius, &button3_relief,
                 NULL, NULL, NULL, NULL,
				 &img3, &ptr_img3_rect, &img3_anchor,
                 &button1_callback, NULL);
	ei_place(button3, NULL, &button3_x, &button3_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Hook the keypress callback to the event. */
	ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL); 

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
	ei_app_free();

	return (EXIT_SUCCESS);
}
