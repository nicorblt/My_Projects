#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_widget.h"
#include "ei_complex_types.h"
#include "widgets/ei_frame.h"

/**
 * @brief Fields are different aspect of a button. To see default values, please refer to \ref ei_button_configure
 *
 */
typedef struct ei_button_t
{
    ei_frame_t frame;       ///< The frame of the button.
    int corner_radius;      ///< The radius of the corner of the button.
    ei_callback_t callback; ///< The callback function to call when the button is pressed.
} ei_button_t;

/**
 * @brief Set the value of a the header-text of the button. Change the button text.
 *
 * @param button The button to modify.
 * @param text The label to write in the button.
 *              A copy is made, so it is safe to release the char on return.
 * @param length length of the "text" given as a parameter.
 *
 */
void ei_button_set_text(ei_button_t *button, const char *text, size_t length);

/**
 * @brief Set the background-image of the button.
 *
 * @param button The button to modify.
 * @param bg_image The background image which we use to draw into the button surface.
 *
 */
void ei_button_set_bg_image(ei_button_t *button, ei_surface_t bg_image);

#endif