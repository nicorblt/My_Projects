#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "helpers/ei_memory.h"


/*
 * main --
 *
 *	Main function of the application.
 */
int main()
{
	ei_size_t	screen_size		= {500, 1000};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_app_create(screen_size, EI_FALSE);

	ei_surface_t 	img1 					= hw_image_load("misc/klimt.jpg", ei_app_root_surface());
	ei_size_t		frame1_size				= {340,340};
	int				frame1_x				= 25;
	int				frame1_y				= 0;
	ei_color_t		frame1_color			= {0xff, 0x60, 0xff, 0xff};
	ei_point_t		point1 					= {0,0};
	ei_size_t 		size_img1 				= {300,300};
	ei_rect_t 		img1_rect 				= {point1,size_img1};
	ei_rect_t 		*ptr_img1_rect 			= &img1_rect;
	ei_anchor_t 	anchor_img1 			= ei_anc_center;
	int 			border_width_frame1 	= 20;
    ei_relief_t 	relief_frame1 			= ei_relief_raised;

	ei_surface_t 		img2 				= hw_image_load("misc/klimt.jpg", ei_app_root_surface());
	int					frame2_x			= 45;
	int					frame2_y			= 345;
	float				frame2_relwidth		= 0.6;
	float				frame2_relheight	= 0.4;
	ei_color_t			frame2_color		= {0xff, 0x60, 0xff, 0x40};
	ei_point_t 			point_img2			= {0,0};
    ei_size_t 			size_img2 			= {300, 300};
    ei_rect_t 			img2_rect 			= {point_img2, size_img2};
    ei_rect_t 			*ptr_img2_rect 		= &img2_rect;
	ei_anchor_t 		anchor_img2			= ei_anc_north;


	ei_size_t		frame3_size				= {470,360};
	float			frame3_relx				= 1;
	float			frame3_rely				= 1;
	int				frame3_x				= -2;
	int				frame3_y				= -2;
	ei_color_t		frame3_color			= {0xe8, 0x98, 0x10, 0xff};
    ei_anchor_t 	anchor_frame3 			= ei_anc_southeast;
	ei_color_t		text3_color				= {0xff, 0xff, 0xff, 0xff};
	char 			*text3 					= "ce texte a pour anchor souteast";
	ei_anchor_t 	text3_anchor			= ei_anc_southeast;
	int 			border_width_frame3 	= 10;
    ei_relief_t 	relief_frame3 			= ei_relief_raised;


	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame on screen. */
	ei_widget_t* frame1 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame1, &frame1_size, &frame1_color,
				&border_width_frame1, &relief_frame1,
				NULL, NULL, NULL, NULL,
				&img1, &ptr_img1_rect, &anchor_img1);
	ei_place(frame1, NULL, &frame1_x, &frame1_y, NULL, NULL, NULL, NULL, NULL, NULL );


	ei_widget_t* frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame2, NULL, &frame2_color,
			    NULL, NULL,
                NULL, NULL, NULL, NULL,
			    &img2, &ptr_img2_rect, &anchor_img2);
	ei_place(frame2, NULL, &frame2_x, &frame2_y, NULL, NULL, NULL, NULL, &frame2_relwidth, &frame2_relheight); //test frame2_x null


	ei_widget_t* frame3 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame3, NULL, &frame3_color,
			    &border_width_frame3, &relief_frame3,
                &text3, NULL, &text3_color, &text3_anchor,
			    NULL, NULL, NULL);
	ei_place(frame3, &anchor_frame3, &frame3_x, &frame3_y, &frame3_size.width, &frame3_size.height, &frame3_relx, &frame3_rely, NULL, NULL);
	
	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();


}