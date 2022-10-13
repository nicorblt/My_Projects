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

	//display an image in the window.
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};
	ei_app_create(screen_size, EI_FALSE);


	ei_size_t	frame1_size			= {200,150};
	int			frame1_x			= 590;
	int			frame1_y			= 110;
	ei_color_t	frame1_color		= {0x88, 0x88, 0x88, 0x40};
    ei_anchor_t anchor_frame1 		= ei_anc_center;

	ei_color_t	text1_color		= {0xff, 0xff, 0xff, 0xff};
	char 		*text1 			= "De la root je dépasse.";
	ei_anchor_t text1_anchor	= ei_anc_south;


	// ei_size_t	frame2_size			= {200,150};
	int				frame2_x			= 300;
	int				frame2_y			= 300;
	ei_color_t		frame2_color		= {0x88, 0x88, 0x88, 0xff};
    ei_anchor_t 	anchor_frame2 		= ei_anc_center;

	ei_color_t	text2_color		= {0xff, 0xff, 0xff, 0xff};
	char 		*text2 			= "Texte au quel la frame adapte sa taille automatiquement.";
	ei_anchor_t text2_anchor 	= ei_anc_south;



	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	// display frame 1, a rogned image for wich one we specify the dimension and use absolut coordinate is put in
	ei_widget_t* frame1 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame1, &frame1_size, &frame1_color,
			    NULL, NULL,
                &text1, NULL, &text1_color, &text1_anchor,
			    NULL, NULL, NULL);
	ei_place(frame1, &anchor_frame1, &frame1_x, &frame1_y, NULL, NULL, NULL, NULL, NULL, NULL );
	
	

	// display frame 2, an image for wich one we don't specify the dimension and use absolut coordinate is put in
	ei_widget_t* frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
	ei_frame_configure(frame2, NULL, &frame2_color,
			    NULL, NULL,
				&text2, NULL, &text2_color, &text2_anchor,
			    NULL, NULL, NULL);
	ei_place(frame2, &anchor_frame2, &frame2_x, &frame2_y, NULL, NULL, NULL, NULL, NULL, NULL );


	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
