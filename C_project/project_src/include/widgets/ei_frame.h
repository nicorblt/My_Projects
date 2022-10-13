#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "ei_widget.h"
#include "ei_complex_types.h"

/**
 * @brief Fields are different aspect of a frame. To see default values, please refer to \ref ei_frame_configure
 *
 */
typedef struct ei_frame_t
{
    ei_widget_t widget; ///< Inheritance of the properties of the base struct \ref ei_widget_t.

    ei_color_t color;   ///< The background color of a frame.
    int border_width;   ///< Width of the border of the frame. Can be 0.
    ei_relief_t relief; ///< Relief of the frame. Optional.

    char *text;                   ///< Label to write in the frame. Can be NULL. Cannot be non-null if "bg_image" is non-null too.
    ei_text_aspect_t text_aspect; ///< Describes the aspect "text", i.e. its style, font, color and anchor.

    ei_surface_t bg_image;               ///< Background image of the frame. Can be NULL. Cannot be non-null if "text" is non-null too.
    ei_surface_aspect_t bg_image_aspect; ///< Defines the aspect of the background image. It defines the relative anchor and the area of the image to display.
} ei_frame_t;

/**
 * @brief Set the value of a the header-text of the frame. Change the frame text.
 *
 * @param frame The frame to modify.
 * @param text The label to write in the frame.
 *              A copy is made, so it is safe to release the char on return.
 * @param length length of the "text" given as a parameter.
 *
 */
void        ei_frame_set_text       (ei_frame_t *frame, const char *text, size_t length);

#endif