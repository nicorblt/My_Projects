#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_utils.h"
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
	
	//display an image in the window.
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_app_create(screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    ei_surface_t quadrant = hw_image_load("misc/quadrant.png", ei_app_root_surface());

	ei_size_t	frame1_size			= {200,150};
	int			frame1_x			= 600;
	int			frame1_y			= 110;
	ei_color_t	frame1_color		= {0x88, 0x88, 0x88, 0xff};
    ei_anchor_t anchor_frame1 		= ei_anc_center;

    ei_point_t 		point_img1 	   = {20,20};
    ei_size_t 		size_img1 	   = {80, 80};
    ei_rect_t		img1_rect 	   = {point_img1, size_img1};
    ei_rect_t 		*ptr_img1_rect = &img1_rect;
    ei_anchor_t 	anchor_img1    = ei_anc_north;


// ei_size_t	frame2_size			= {200,150};
	int			frame2_x			= 300;
	int			frame2_y			= 300;
	ei_color_t	frame2_color		= {0x88, 0x88, 0x88, 0xff};


    ei_point_t  point_img2     = {0,0};
    ei_size_t	size_img2 	   = {90, 90};
    ei_rect_t 	img2_rect 	   = {point_img2, size_img2};
    ei_rect_t 	*ptr_img2_rect = &img2_rect;

    ei_anchor_t anchor_frame2 = ei_anc_center;
    ei_anchor_t anchor_img2	  = ei_anc_south;

	// ei_size_t	frame3_size		= {200,150};
	float		frame3_relx			= 0.2;
	float		frame3_rely			= 0.95;
	ei_color_t	frame3_color		= {0x88, 0x88, 0x88, 0xff};

    ei_point_t	point_img3		 		= {0,0};
    ei_size_t 	size_img3 				= {100, 100};
    ei_rect_t 	img3_rect 				= {point_img3, size_img3};
    ei_rect_t 	*ptr_img3_rect 			= &img3_rect;
	int 		border_width_frame3 	= 5;
    ei_relief_t relief_frame3 			= ei_relief_raised;

    ei_anchor_t anchor_frame3	= ei_anc_center;
    ei_anchor_t anchor_img3 	= ei_anc_center;


	// display frame 1, a rogned image for wich one we specify the dimension and use absolut coordinate is put in
	/* Create, configure and place the frame1 on screen. */
	ei_widget_t* frame1 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame1, &frame1_size, &frame1_color,
			    NULL, NULL,
                NULL, NULL, NULL, NULL,
			    &quadrant, &ptr_img1_rect, &anchor_img1);
	ei_place(frame1, &anchor_frame1, &frame1_x, &frame1_y, NULL, NULL, NULL, NULL, NULL, NULL );
	
	

	// display frame 2, an image for wich one we don't specify the dimension and use absolut coordinate is put in
	/* Create, configure and place the frame2 on screen. */
	ei_widget_t* frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame2, NULL, &frame2_color,
			    NULL, NULL,
                NULL, NULL, NULL, NULL,
			    &quadrant, &ptr_img2_rect, &anchor_img2);
	ei_place(frame2, &anchor_frame2, &frame2_x, &frame2_y, NULL, NULL, NULL, NULL, NULL, NULL );


	// display frame 3, an image for wich one we don't specify the dimension and use relative coordinate is put in
	/* Create, configure and place the frame3 on screen. */
	ei_widget_t* frame3 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame3, NULL, &frame3_color,
			    &border_width_frame3, &relief_frame3,
                NULL, NULL, NULL, NULL,
			    &quadrant, &ptr_img3_rect, &anchor_img3);
	ei_place(frame3, &anchor_frame3, NULL, NULL, NULL, NULL, &frame3_relx, &frame3_rely, NULL, NULL );


	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
