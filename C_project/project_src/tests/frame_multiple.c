#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "helpers/ei_memory.h"

ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
	return EI_TRUE;
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


	ei_size_t	frame1_size			= {100,100};
	int			frame1_x			= 75;
	int			frame1_y			= 75;
	ei_color_t	frame1_color		= {0x88, 0x88, 0x88, 0xff};

	ei_surface_t 	img1			 = hw_image_load("misc/klimt.jpg", ei_app_root_surface());
	ei_point_t 		zero			 = {100,100};
	ei_size_t 		size_img1		 = {100,100};
	ei_rect_t 		img1_rect 		 = {zero,size_img1};
	ei_rect_t 		*ptr_img1_rect	 = &img1_rect;
	ei_anchor_t 	img1_anchor 	 = ei_anc_northwest;


	ei_size_t	frame2_size			= {400,400};
	int			frame2_x			= 100;
	int			frame2_y			= 200;
	ei_color_t	frame2_color		= {0xe1, 0xeb, 0x34, 0x20};

	ei_color_t		text2_color		= {0xff, 0xff, 0xff, 0xff};
	char 			*text2 			= "Texte qui dépasse du frame malheureusement";
	ei_anchor_t 	text2_anchor 	= ei_anc_northwest;


	ei_size_t		frame3_size			= {50,50};
	int				frame3_x			= 10;
	int				frame3_y			= 0;
	ei_color_t		frame3_color		= {0x88, 0x88, 0x88, 0x40};
	ei_color_t		text3_color			= {0xff, 0x00, 0xff, 0xff};
	char 			*text3 				= "ben";
	ei_anchor_t 	text3_anchor		= ei_anc_northwest;


	ei_widget_t*	button4;
	ei_size_t		button4_size		= {200,200};
	int				button4_x			= 100;
	int				button4_y			= 100;
	ei_color_t		button4_color		= {0x00, 0xff, 0x00, 0xA0};

	char*			button4_title		= "Bouton dans frame";
	ei_color_t		button4_text_color	= {0x00, 0x00, 0x00, 0xff};
	ei_anchor_t		button4_text_anchor	= ei_anc_center;

	int				button4_corner_radius	= 30;
	ei_relief_t		button4_relief		= ei_relief_raised;
	int				button4_border_width	= 6;
	ei_callback_t	button4_callback = button_press;

	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	// first frame
	/* Create, configure and place the frame on screen. */
	ei_widget_t* frame1 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame1, &frame1_size, &frame1_color,
				NULL, NULL,
				NULL, NULL, NULL, NULL,
				&img1, &ptr_img1_rect, &img1_anchor);
	ei_place(frame1, NULL, &frame1_x, &frame1_y, NULL, NULL, NULL, NULL, NULL, NULL );


	// 		TEXT //
	ei_widget_t* frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame2, &frame2_size, &frame2_color,
				NULL, NULL, &text2, NULL, &text2_color, &text2_anchor,
				NULL, NULL, NULL);
	ei_place(frame2, NULL, &frame2_x, &frame2_y, NULL, NULL, NULL, NULL, NULL, NULL );


	//		CHILD
	ei_widget_t* frame3 = ei_widget_create("frame", frame1, NULL, NULL);
	ei_frame_configure(frame3, &frame3_size, &frame3_color,
				NULL, NULL, &text3, NULL, &text3_color, &text3_anchor,
				NULL, NULL, NULL);
	ei_place(frame3, NULL, &frame3_x, &frame3_y, NULL, NULL, NULL, NULL, NULL, NULL );

	button4 = ei_widget_create("button", frame2, NULL, NULL);
	ei_button_configure	(button4, &button4_size, &button4_color,
				 &button4_border_width, &button4_corner_radius, &button4_relief,
                 &button4_title, NULL, &button4_text_color, &button4_text_anchor,
				 NULL, NULL, NULL,
                 &button4_callback, NULL);
	ei_place(button4, NULL, &button4_x, &button4_y, NULL, NULL, NULL, NULL, NULL, NULL );


	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();


}