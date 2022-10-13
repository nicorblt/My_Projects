#ifndef EI_COMPLEX_TYPES_H
#define EI_COMPLEX_TYPES_H

#include "ei_types.h"

/**
 * @brief Fields are different aspects that decorate a text.
 * 
 */
typedef struct ei_text_aspect_t {
    ei_font_t font; ///< The font used to render the text. Remember to free it once it is not used anymore.
    ei_color_t color; ///< The text color.
    ei_fontstyle_t style; ///< Style of text, e.g. bold. Refs \ref ei_fontstyle_t for values.
    ei_anchor_t anchor; ///< Anchor is from where to apply transformation to the text.
} ei_text_aspect_t;

/**
 * @brief Fields are different aspect of a surface,
 * 
 */
typedef struct ei_surface_aspect_t {
    ei_anchor_t anchor; ///< Anchor is from where to apply transformation to the text.
    ei_rect_t *area; ///< Area describes which section of the surface to display. Remember to free it.
} ei_surface_aspect_t;

#endif