#ifndef EI_TOPLEVELCLASS_CONFIG_H
#define EI_TOPLEVELCLASS_CONFIG_H

#include "widgets/ei_toplevel.h"

static int k_bottom_right_rectangle_width = 15;                       ///< The width of the bot right corner button for resize.
static int k_radius = 25;                                             ///< The radius of both rounded top corner
static ei_color_t k_head_strip_text_color = {0xff, 0xff, 0xff, 0xff}; ///< The color of title of toplevel
static ei_color_t k_strip_button_color = {0xff, 0x00, 0x00, 0xff};    ///< The color of the strip button
static ei_color_t k_color_border = {0x20, 0x20, 0x20, 0xff};          ///< The color of border

static ei_size_t k_toplevel_close_button_size = {(int)(5 * (k_toplevel_strip_height >> 3)), (int)(5 * (k_toplevel_strip_height >> 3))};
static int k_toplevel_close_button_corner_radius = (int)(5 * (k_toplevel_strip_height >> 4));
static int k_close_border_width = (int)(5 * (k_toplevel_strip_height >> 3) * 0.1);
#endif