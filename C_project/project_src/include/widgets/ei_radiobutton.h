#ifndef EI_RADIOBUTTON_H
#define EI_RADIOBUTTON_H

#include "ei_widget.h"
#include "widgets/ei_frame.h"

struct circle_properties
{
    int radius;
};

struct square_properties
{
    int size;
};

struct rectangle_properties
{
    int width;
    int height;
};

struct diamond_properties
{
    int size;
};

struct triangle_properties
{
    int base;
    int height;
};

/**
 * @brief	Enum type for the radiobutton's different shape.
 *
 */
typedef enum ei_type_shape_t
{
    ei_shape_circle = 0, ///< The shape of the radiobutton is a circle.
    ei_shape_square,     ///< The shape of the radiobutton is a square.
    ei_shape_rectangle,  ///< The shape of the radiobutton is a rectangle with a width and height given.
    ei_shape_diamond,    ///< The shape of the radiobutton is a diamond which just needs an size = width = height.
    ei_shape_triangle,   ///< The shape of the radiobutton is a triangle which has a base and an height.
    ei_shape_invalid      ///< Invalid.
} ei_type_shape_t;

/**
 * @brief	Structure for the radiobutton's shape.
 * Uses the \ref ei_type_shape_t enum to define the shape of the radiobutton.
 *
 */
typedef struct ei_shape_t
{
    ei_type_shape_t type; ///< The type of the shape; can be a circle, a square, a rectangle, a diamond, a triangle.
    union {
        struct  circle_properties      circle;
        struct  square_properties      square;
        struct  rectangle_properties   rectangle;
        struct  diamond_properties     diamond;
        struct  triangle_properties    triangle;
    } properties;
} ei_shape_t;

typedef struct ei_radiobutton_form_t
{
    struct ei_radiobutton_t *head;
    struct ei_radiobutton_t *tail;
} ei_radiobutton_form_t;

/**
 * @brief	Configures the attributes of widgets of the class "radiobutton".
 *
 */
typedef struct ei_radiobutton_t
{
    ei_frame_t frame;       ///< Inheritance of the properties of the base struct \ref ei_frame_t.
    ei_shape_t shape;       ///< The shape of the radiobutton. See \ref ei_type_shape_t.
    ei_bool_t selected;     ///< The radiobutton is selected or not.
    ei_color_t fill_color;  ///< The fill color of the radiobutton which can be different from the one of the frame.
    ei_callback_t callback; ///< The callback function to call when the radiobutton is clicked.
    int id;                 ///< The id of the radiobutton.
    struct ei_radiobutton_form_t *form; ///< The form the radiobutton is in.
    struct ei_radiobutton_t *next; ///< The next radiobutton in the form.
} ei_radiobutton_t;

static const int k_default_radiobutton_border_width = 10;   ///< The default border width of radiobutton widgets.

/**
 * @brief	Configures the attributes of widgets of the class "radiobutton".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. For the radiobutton, there is no background image. However, we provide a shape. The information "selected" is set to false by default.
 * @param	callback	The callback function to call when the user clicks on one radiobutton.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
void        ei_radiobutton_configure        (ei_widget_t *widget,
                                            ei_size_t *requested_size,
                                            const ei_color_t *color,
                                            const ei_color_t *fill_color,
                                            int *border_width,
                                            ei_relief_t *relief,
                                            ei_type_shape_t *shape,
                                            ei_bool_t *is_selected,
                                            char **text,
                                            ei_font_t *text_font,
                                            ei_color_t *text_color,
                                            ei_anchor_t *text_anchor,
                                            ei_callback_t *callback,
                                            void **user_param);

/**
 * @brief   Add a radiobutton to the given radiobutton linked list.
 *
 * @param   head   The head of the linked list; this head will be remplaced by the new radiobutton.
 * @param   to_append     The radiobutton to add (cannot be NULL and must be out of any form).
 */
void        ei_radiobutton_form_append      (ei_radiobutton_form_t **form,
                                            ei_radiobutton_t *to_append);

/**
 * @brief   Set the radiobutton with the given id to selected and all others to not selected selected state.
 * NOTE : assume that the id given is valid (i.e. exists in the linked list of radio button).
 *
 * @param   radiobutton   A radio button.
 * @param   id            The id of the radio button to set to selected.
 */
void        ei_radiobutton_set_selected         (ei_radiobutton_form_t *form, 
                                                int id);

#endif