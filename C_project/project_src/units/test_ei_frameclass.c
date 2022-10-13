/**
 * @file test_ei_frameclass.c
 * @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 * @brief Module for testing ei_frameclass.h functions.
 * Testing list: ei_frame_drawer
 * @version 0.1
 * @date 2022-05-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ei_widget.h"
#include "ei_types.h"
#include "widgetclasses/ei_frameclass.h"
#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_application.h"

void test_ei_frame_drawer();

int main()
{
    test_ei_frame_drawer();
    return EXIT_SUCCESS;
}

void test_ei_frame_drawer()
{
    /* Create the frame */
    ei_size_t window_size = {100, 100};
    ei_size_t widget_size = {50, 50};
    ei_color_t color = {0x00, 0x00, 0xff, 0x2A}; // blue 42/255 opaque
    ei_color_t bg_color = {0xff, 0x00, 0x00, 0xff};
    ei_anchor_t anchor = ei_anc_southeast;
    ei_anchor_t anchor_bis = ei_anc_northwest;

    ei_rect_t clipper;
    clipper.size = window_size;
    clipper.top_left = ei_point(0, 0);
    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 1 : draw a frame inside the root window (blue frame)");
        ei_frame_configure(ei_app_root_widget(), NULL, &bg_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_frame_configure(widget, &widget_size, &color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        puts("\t OK");

        ei_app_run();
        ei_app_free();
    }

    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 2 : draw a frame part outside the root window (green frame)");
        widget_size.width = 200;
        color.green = 0xff;
        color.blue = 0x00;
        ei_frame_configure(widget, &widget_size, &color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_frame_drawer(widget, root_surface, pick_surface, &clipper);
        puts("\t OK");

        ei_app_run();
        ei_app_free();
    }

    {
        printf("\nTest 3 : draw a frame outside the root window");
        widget_size.width = 50;
        puts("\t Not Found");
    }

    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 4 : draw a frame on top of another frame to see alpha blending (red frame)");
        color.green = 0x00;
        color.red = 0xff;
        ei_frame_configure(widget, &widget_size, &color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_frame_drawer(widget, root_surface, pick_surface, &clipper);
        puts("\t OK");

        ei_app_run();
        ei_app_free();
    }

    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 4 : draw a frame on top of another frame to see alpha blending (red frame)");
        color.green = 0x00;
        color.red = 0xff;
        ei_frame_configure(widget, &widget_size, &color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_frame_drawer(widget, root_surface, pick_surface, &clipper);
        puts("\t OK");

        ei_app_run();
        ei_app_free();
    }

    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 5 : draw a frame with a text inside");
        char *text = "yo man ! cool :D";
        ei_color_t text_red = {0xff, 0x00, 0x00, 0xff};
        ei_font_t font = ei_default_font;

        color.green = 0xff;
        ei_frame_configure(widget, &widget_size, &color, NULL, NULL, &text, &font, &text_red, &anchor, NULL, NULL, NULL);
        ei_frame_drawer(widget, root_surface, pick_surface, &clipper);
        puts("\t OK");

        ei_app_run();
        ei_app_free();
    }

    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 6 : draw a frame with a background image");
        ei_surface_t quadrant_img;
        if ((quadrant_img = hw_image_load("misc/quadrant.png", root_surface)) == NULL)
        {
            printf("\nERROR: could not load image \"misc/quadrant.png\"");
            exit(1);
        }
        color.blue = 0xff;
        ei_rect_t *clipper_ptr = &clipper;
        ei_frame_configure(widget, &widget_size, &color, NULL, NULL, NULL, NULL, NULL, NULL, quadrant_img, &clipper_ptr, &anchor_bis);
        ei_frame_drawer(widget, root_surface, pick_surface, &clipper);
        puts("\t OK");

        ei_app_run();
        hw_surface_free(quadrant_img);
        ei_app_free();
    }

    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 7 : draw a frame with relief raised");
        widget_size.width = widget_size.height = 20;
        color.red = 0x5F;
        ei_relief_t relief = ei_relief_raised;
        int border_width = 5;
        char *text_btn_1 = "BTN 1";
        ei_color_t text_green = {0x00, 0xff, 0x00, 0xff};
        ei_font_t font = ei_default_font;
        ei_frame_configure(widget, &widget_size, &color, &border_width, &relief, &text_btn_1, &font, &text_green, NULL, NULL, NULL, NULL);
        ei_frame_drawer(widget, root_surface, pick_surface, &clipper);
        puts("\t OK");

        ei_app_run();
        ei_app_free();
    }

    {
        ei_app_create(window_size, EI_FALSE);
        ei_surface_t root_surface = ei_app_root_surface();
        ei_surface_t pick_surface = hw_surface_create(root_surface, window_size, EI_TRUE);
        ei_widget_t *widget = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        printf("\nTest 8 : draw a frame with relief sunken");
        color.green = 0x5F;
        ei_relief_t relief = ei_relief_sunken;
        int border_width = 5;
        char *text_btn_2 = "BTN 2";
        ei_color_t text_blue = {0x00, 0x00, 0xff, 0xff};
        ei_font_t font = ei_default_font;
        ei_frame_configure(widget, &widget_size, &color, &border_width, &relief, &text_btn_2, &font, &text_blue, NULL, NULL, NULL, NULL);
        ei_frame_drawer(widget, root_surface, pick_surface, &clipper);
        puts("\t OK");

        ei_app_run();
        ei_app_free();
    }

    puts("ei_frame_drawer OK.");
}
