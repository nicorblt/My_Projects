/**
 * @file test_ei_radiobutton.c
 * @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 * @brief Module for testing ei_radiobuttonclass.h functions.
 * Testing list: ei_button_drawer
 * @version 0.1
 * @date 2022-05-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdlib.h>

#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "widgetclasses/ei_radiobuttonclass.h"
#include "widgets/ei_radiobutton.h"
#include "ei_geometrymanager.h"
#include "ei_application.h"

void test_ei_radiobutton_drawer();
void test_ei_radiobutton_form_append();

int main()
{
    // test_ei_radiobutton_form_append();
    test_ei_radiobutton_drawer();
    return EXIT_SUCCESS;
}

ei_bool_t process_key(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    if (event->param.key.key_code == SDLK_ESCAPE)
    {
        ei_app_quit_request();
        return EI_TRUE;
    }

    return EI_FALSE;
}

ei_bool_t button_press(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    ei_radiobutton_t *radiobutton = (ei_radiobutton_t *)widget;
    ei_radiobutton_set_selected(radiobutton->form, radiobutton->id);
    printf("Click on id=%d\n", radiobutton->id);
    return EI_TRUE;
}

void test_ei_radiobutton_form_append()
{
    ei_app_create((ei_size_t){600, 600}, EI_FALSE);

    ei_frame_configure(ei_app_root_widget(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    ei_radiobutton_form_t *form = NULL;
    ei_radiobutton_t *to_append = (ei_radiobutton_t *)ei_widget_create("radiobutton", ei_app_root_widget(), NULL, NULL);
    ei_radiobutton_form_append(&form, to_append);

    ei_radiobutton_t *to_append_bis = (ei_radiobutton_t *)ei_widget_create("radiobutton", ei_app_root_widget(), NULL, NULL);
    ei_radiobutton_form_append(&form, to_append_bis);

    ei_app_free();
}

void test_ei_radiobutton_drawer()
{
    ei_size_t screen_size = {600, 600};
    ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};

    ei_widget_t *button;
    ei_radiobutton_t *radio;
    int button_x, button_y, x, y;
    button_x = button_y = x = y = 40;
    ei_color_t button_color = {0xda, 0xda, 0xda, 0xff};
    ei_color_t fill_color = {0x2a, 0x2a, 0x2a, 0xff};
    char *btn_name = "Button";
    ei_anchor_t anchor = ei_anc_center;
    ei_type_shape_t shape = ei_shape_circle;
    ei_callback_t button_callback = button_press;

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




    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the button on screen. */
	toplevel1 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure	(toplevel1, NULL, &toplevel1_color,
				 &toplevel1_border_width, &toplevel1_title, &toplevel1_closable, &toplevel1_resizable, &ptr_toplevel1_min_size);
	ei_place(toplevel1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
    
    ei_radiobutton_form_t *form = NULL;

    for (int i = 0; i < 10; i++)
    {
        y += 50;
        button = ei_widget_create("radiobutton", toplevel1, NULL, NULL);
        radio = (ei_radiobutton_t *)button;
        ei_radiobutton_configure(button, &(ei_size_t){button_x, button_y}, &button_color, &fill_color, &i, NULL, &shape, NULL, &btn_name, NULL, NULL, &anchor, &button_callback, NULL);
        ei_radiobutton_form_append(&form, radio);
        ei_place(button, &anchor, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL);
    }
/* 
    ei_radiobutton_set_selected(form, 2);
    ei_radiobutton_set_selected(form, 9);
    ei_radiobutton_set_selected(form, 10);
 */
    ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);
    ei_app_run();
    ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);
    ei_app_free();
}
