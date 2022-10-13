#ifndef EI_GEOMETRY_HELPERS_H
#define EI_GEOMETRY_HELPERS_H

#include "ei_types.h"
#include "hw_interface.h"
#include "helpers/ei_math.h"
#include "ei_widget.h"

struct ei_linked_rect_t;
struct ei_linked_point_t;
// TODO améliorer linked_point en ajoutant un superviseur qui remémore la tete head et la queue tail de la liste (aisni linked_join en O(1))

/**
 * @brief   Free the memory used by a linked rect.
 *
 * @param   first_link   The first link of the linked rect to be freed.
 */
void ei_linked_rect_destroy(struct ei_linked_rect_t * first_link);

/**
 * @brief   Free the memory used by a linked point.
 *
 * @param   first_link   The first link of the linked point to be freed.
 */
void ei_linked_point_destroy(struct ei_linked_point_t * first_link);

/**
 * @brief  Add a new point on the head of a linked list.
 *
 * @param   list    The linked point list that will recieve the new point.
 * @param   point   The new point that will be appended at the head of the list.
 *
 * @return  The list of point with the new point.
 *
 */
ei_linked_point_t       *ei_linked_point_add        (ei_linked_point_t *list,
									                ei_point_t point);

/**
 * @brief  Join the head of the second linked_list to the tail of the first.
 *
 * @param   first    The linked point which have tail jointed to the head of second.
 * @param   second   The linked point which have head jointed to the tail of first.
 *
 * eg: first = 1 -> 2 -> 3
 *     second = 4 -> 5
 *     result = 4 -> 5 -> 1 -> 2 -> 3
 * @return  The new list of point joined.
 *
 */
ei_linked_point_t       *ei_linked_point_join        (ei_linked_point_t *first,
									                 ei_linked_point_t *second);

/**
 * @brief   Free the memory used by a linked list of points.
 * 
 * @param   list    The linked list of points to free.
 */
void        free_linked_list        (ei_linked_point_t *list);

/**
 * @brief   Computes the list of point that compose the arc of the corner of a widget.
 *
 * @param   center         The center of the arc.
 * @param   radius         The radius of the arc.
 * @param   angle_start    The start angle of the arc (in radian).
 * @param   angle_end      The end angle of the arc (in radian).
 *
 * @return  The list of point that compose the arc of a corner.
 *
 */
ei_linked_point_t       *arc        (ei_point_t center,
                                    uint32_t radius,
                                    double angle_start,
                                    double angle_end);

/**
 * @brief   Computes the list of point that compose the frame with arched corners of a widget.
 * 
 * @param   rect   The rect that has to be rounded.
 * @param   radius   The radius of the arc.
 * @param   side   The side of the rectangle that has to be computed.
 * If "all" then the function will compute the list of points for all the frame.
 * If "upper" then the function will compute the list of points for the upper frame.
 * If "lower" then the function will compute the list of points for the lower frame.
 * For more information, see p. 42 A5.   
 * 
 * @return  The list of point that compose the frame of a widget.
 * 
 */
ei_linked_point_t       *rounded_frame       (ei_rect_t rect, \
                                             int radius, \
                                             char* side);

/**
 * @brief   Computes the list of point that compose the frame with arched corners of a widget.
 * 
 * @param   rect   The rectangle that has to be rounded.
 * @param   radius   The radius of the arc.
 * @param   side   The side of the rectangle that has to be computed.
 * If "all" then the function will compute the list of points for all the frame.
 * If "upper" then the function will compute the list of points for the upper frame.
 * If "lower" then the function will compute the list of points for the lower frame.
 * @param   border_width   The width of the border.
 * 
 * @return  The list of point that compose the frame of a widget.
 * 
 */
ei_linked_point_t       *rounded_frame_borders       (ei_rect_t rect, 
                                                     int radius, 
                                                     char* side,
                                                     int border_width);


/**
 * @brief   Computes the list of point that compose a plain frame with arched corners of a widget.
 * 
 * @param   rect   The rectangle that has to be rounded.
 * @param   radius   The radius of the arcs.
 * 
 * @return  The list of point that compose the frame of a widget.
 * 
 */
ei_linked_point_t       *rounded_frame_plain        (ei_rect_t rect,
                                                    int radius);
/**
 * @brief   Computes the list of point that compose a rectangle of given size.
 * 
 * @param   size   The size (width and height) of the rectangle.
 * @param   top_left   The coords of the top left corner of the rectangle.
 * 
 * @return  The list of point that compose the rectangle.
 * 
 */
ei_linked_point_t       *ei_linked_point_rectangle      (ei_size_t size, ei_point_t top_left);

/**
 * @brief   Computes the list of point that compose a square of given size.
 * 
 * @param   size   The size of the square.
 * @param   top_left   The coords of the top left corner of the square.
 * 
 * @return  The list of point that compose the square.
 * 
 */
ei_linked_point_t       *ei_linked_point_square     (int size, ei_point_t top_left);

/**
 * @brief   Computes the list of point that compose a diamond of given size.
 * 
 * @param   size   The size (it's a quare rotated by 45 degrees) of the diamond.
 * @param   top_left   The coords of the top corner of the diamond.
 * 
 * @return  The list of point that compose the diamond.
 * 
 */
ei_linked_point_t       *ei_linked_point_diamond        (int size, ei_point_t top_left);

/**
 * @brief   Computes the list of point that compose a triangle of given size.
 * 
 * @param   size   The size (base and height) of the triangle.
 * @param   top_left   The coords of the top corner of the triangle.
 * 
 * @return  The list of point that compose the triangle.
 * 
 */
ei_linked_point_t       *ei_linked_point_triangle       (ei_size_t size, ei_point_t top_left);

/**
 * @brief Return ei_point_t associated to anchor and surface.
 * the point returned represent the coordinate of the anchor relatively to the surface and placed object.
 *
 * @param anchor A relative anchor point inside the rect.
 * @param rect rect to get the point from.
 * @param anchored_size size of the placed object.
 *
 * @return ei_point_t point associated to coordinates of the anchor and surface.
 *
 */
ei_point_t      ei_get_coords_from_anchor       (ei_anchor_t anchor, 
                                                 const ei_rect_t *rect_container,
                                                 const ei_size_t  anchored_size);

/**
 * @brief   Return the intersection between two rectangles.
 *
 * @param   lhs     A first rectangle.
 * @param   rhs     A second rectangle.
 *
 * @return  The intersection rectangle.
 *
 */
ei_rect_t       intersect_rect       (const ei_rect_t *to_clip,
                                 const ei_rect_t *clipper);



// INLINE Functions
/**
 * @brief   Compare two size struct \ref ei_size_t
 * 
 * @param   lhs    Left size.
 * @param   rhs    Right size.
 * @return  \ref int Negative value if lhs area is greater than rhs area, 
 * 0 if equal, positive if rhs has greater area than lhs.
 */
static inline int   _ei_compare_size    (ei_size_t lhs, ei_size_t rhs)
{
    if (lhs.height == rhs.height && lhs.width == rhs.width) // save a multiplication if possible
    {
        return 0;
    }
	return ABS(rhs.height * rhs.width) - ABS(lhs.height * lhs.width);
}
#define EI_COMPARE_SIZE(lhs, R, rhs) ( _ei_compare_size((lhs), (rhs)) R 0)

/**
 * @brief    Compare two rectangles \ref ei_rect_t
 * 
 * @param    lhs    Left rectangle.
 * @param    rhs    Right rectangle.
 * 
 * @return   \ref ei_bool_t EI_TRUE if both rectangles are equals in position and size.
 * EI_FALSE otherwise.
 */
static inline ei_bool_t     _ei_compare_rect    (ei_rect_t lhs, ei_rect_t rhs)
{
    return EI_COMPARE_SIZE(lhs.size, ==, rhs.size) \
     && lhs.top_left.x == rhs.top_left.x \
     && lhs.top_left.y == rhs.top_left.y;
}
#define EI_COMPARE_RECT(lhs, R, rhs) ( _ei_compare_rect((lhs), (rhs)) R EI_TRUE)

/**
 * @brief    Check if a point is inside a rectangle.
 * 
 * @param    point  The point to check.
 * @param    rect   The rectangle that contains or not the point.
 * 
 * @return   ei_bool_t EI_TRUE if the point is inside the rectangle and EI_FALSE otherwise.
 */
static inline ei_bool_t ei_point_is_in_rect(ei_point_t point, ei_rect_t rect)
{
    return (point.x >= rect.top_left.x) && (point.x < rect.top_left.x + rect.size.width) \
        && (point.y >= rect.top_left.y) && (point.y < rect.top_left.y + rect.size.height);
}

#endif