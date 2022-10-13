#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "helpers/ei_geometry_helpers.h"
#include "ei_types.h"
#include "ei_utils.h"
#include "helpers/ei_memory.h"
#include "helpers/ei_string.h"

ei_linked_point_t *ei_linked_point_add(ei_linked_point_t *list,
									   ei_point_t point)
{
	ei_linked_point_t *new_point = verified_malloc(sizeof(ei_linked_point_t));
	new_point->point = point;
	new_point->next = list;
	return new_point;
}

void free_linked_list(ei_linked_point_t *list)
{
	ei_linked_point_t *current = list;
	ei_linked_point_t *next;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

ei_linked_point_t *ei_linked_point_join(ei_linked_point_t *first,
										ei_linked_point_t *second)
{
	ei_point_t zero = {0, 0};
	ei_linked_point_t sent = {zero, first};
	ei_linked_point_t *current = &sent;

	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = second;
	return sent.next;
}

ei_linked_point_t *arc(ei_point_t center,
					   uint32_t radius,
					   double angle_start,
					   double angle_end)
{
	ei_linked_point_t *list = NULL;

	// Always choose the shortest path
	// angle_end = fmod(angle_end, 2 * M_PI); //enlever pourle cercle du toplevel (je pense pas necessaire de mettre un modulo)
	// angle_start = fmod(angle_start, 2 * M_PI);

	uint32_t nb_points = (size_t)((ABS(angle_end - angle_start) * 100) / (2 * M_PI)) + 1; // Assuming 100 points = one entire circle
	double pas = (angle_end - angle_start) / nb_points;

	for (uint32_t i = 0; i <= nb_points; i++)
	{
		ei_point_t point = {center.x + radius * cos(angle_start + i * pas), center.y - radius * sin(angle_start + i * pas)};
		list = ei_linked_point_add(list, point);
	}

	return list;
}

ei_linked_point_t *rounded_frame(ei_rect_t rect,
								 int radius,
								 char *side)
{
	int height = rect.size.height;
	int width = rect.size.width;
	int topleft_x = rect.top_left.x;
	int topleft_y = rect.top_left.y;
	ei_linked_point_t *listed_point = NULL;
	ei_point_t point_L;
	ei_point_t point_K;

	ei_point_t center_top_left = {topleft_x + radius, topleft_y + radius};
	ei_point_t center_bottom_right = {topleft_x + width - radius, topleft_y + height - radius};
	if (height < width)
	{
		// L point
		point_L.x = topleft_x + width - (int)height / 2;
		point_L.y = topleft_y + (int)height / 2;

		// K point
		point_K.x = topleft_x + (int)height / 2;
		point_K.y = topleft_y + (int)height / 2;
	}
	else
	{
		// L point
		point_L.x = topleft_x + (int)width / 2;
		point_L.y = topleft_y + (int)width / 2;

		// K point
		point_K.x = topleft_x + (int)width / 2;
		point_K.y = topleft_y + height - (int)width / 2;
	}

	// A point
	ei_point_t point_A = {topleft_x + (int)(radius * (1 - sqrt(2) / 2)), topleft_y + height - (int)(radius * (1 - sqrt(2) / 2))};

	if (STRCMP(side, ==, "upper") || STRCMP(side, ==, "all"))
	{ // create the upper side
		// add  B <- A
		ei_linked_point_t *bottom_left_upper_arc = arc((ei_point_t){center_top_left.x, center_bottom_right.y}, radius, 5 * M_PI / 4, M_PI);
		listed_point = ei_linked_point_join(bottom_left_upper_arc, listed_point);

		// add  D <- C <- B
		ei_linked_point_t *top_left_arc = arc(center_top_left, radius, M_PI, M_PI / 2);
		listed_point = ei_linked_point_join(top_left_arc, listed_point);

		// add  F <- E <- D
		ei_linked_point_t *top_right_upper_arc = arc((ei_point_t){center_bottom_right.x, center_top_left.y}, radius, M_PI / 2, M_PI / 4);
		listed_point = ei_linked_point_join(top_right_upper_arc, listed_point);
	}
	else if (STRCMP(side, ==, "lower"))
	{ // create the lower side
		// add  J <- A
		ei_linked_point_t *bottom_left_lower_arc = arc((ei_point_t){center_top_left.x, center_bottom_right.y}, radius, 5 * M_PI / 4, 3 * M_PI / 2);
		listed_point = ei_linked_point_join(bottom_left_lower_arc, listed_point);

		// add  H <- I <- J
		ei_linked_point_t *bottom_right_arc = arc(center_bottom_right, radius, -M_PI / 2, 0);
		listed_point = ei_linked_point_join(bottom_right_arc, listed_point);

		// add  F <- G <- H
		ei_linked_point_t *top_right_upper_arc = arc((ei_point_t){center_bottom_right.x, center_top_left.y}, radius, 0, M_PI / 4);
		listed_point = ei_linked_point_join(top_right_upper_arc, listed_point);
	}
	if (STRCMP(side, ==, "all"))
	{
		// add G <- F <- F
		ei_linked_point_t *end_upper_right_arc = arc((ei_point_t){center_bottom_right.x, center_top_left.y}, radius, M_PI / 4, 0);
		listed_point = ei_linked_point_join(end_upper_right_arc, listed_point);

		// add I <- H <- G
		ei_linked_point_t *end_lower_right_arc = arc(center_bottom_right, radius, 0, -M_PI / 2);
		listed_point = ei_linked_point_join(end_lower_right_arc, listed_point);

		// add A <- J <- I
		ei_linked_point_t *end_lower_left_arc = arc((ei_point_t){center_top_left.x, center_bottom_right.y}, radius, 3 * M_PI / 2, 5 * M_PI / 4);
		listed_point = ei_linked_point_join(end_lower_left_arc, listed_point);

		return listed_point;
	}

	// add F -> L
	listed_point = ei_linked_point_add(listed_point, point_L);
	// add L -> K
	listed_point = ei_linked_point_add(listed_point, point_K);
	// add K -> A
	listed_point = ei_linked_point_add(listed_point, point_A);
	return listed_point;
}

/**
 * @brief   Computes the list of point that compose the frame with arched corners of a widget.
 *
 * @param   rect   The that has to be rounded.
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
ei_linked_point_t *rounded_frame_borders(ei_rect_t rect,
										 int radius,
										 char *side,
										 int border_width)
{
#ifndef RELEASE
	//assert(border_width <= radius && "radius can not be inferior to border_width.\n");
#endif

	int height = rect.size.height;
	int width = rect.size.width;
	int topleft_x = rect.top_left.x;
	int topleft_y = rect.top_left.y;
	ei_linked_point_t *listed_point = NULL;

	ei_point_t center_outer_top_left = {topleft_x + radius, topleft_y + radius};
	ei_point_t center_outer_bottom_left = {topleft_x + radius, topleft_y + height - radius};
	ei_point_t center_outer_top_right = {topleft_x + width - radius, topleft_y + radius};
	ei_point_t center_outer_bottom_right = {topleft_x + width - radius, topleft_y + height - radius};

	// Add the last A to list
	ei_point_t point_A;
	point_A.x = topleft_x + (int)(radius * (1 - sqrt(2) / 2));
	point_A.y = topleft_y + height - (int)(radius * (1 - sqrt(2) / 2));
	listed_point = ei_linked_point_add(listed_point, point_A); // A

	if (STRCMP(side, ==, "upper"))
	{ // create the upper side (A<-B<-C<-D<-E<-F<-G<-H<-I<-J<-K<-L<-A)
		// add  K <- L <- A
		ei_linked_point_t *bottom_inner_left_arc = arc(center_outer_bottom_left, radius - border_width, 5 * M_PI / 4, M_PI); // K <- L
		listed_point = ei_linked_point_join(bottom_inner_left_arc, listed_point);

		// add I <- J <- K
		ei_linked_point_t *upper_inner_left_arc = arc(center_outer_top_left, radius - border_width, M_PI, M_PI / 2); // I <- J
		listed_point = ei_linked_point_join(upper_inner_left_arc, listed_point);

		// add G <- H <- I
		ei_linked_point_t *upper_inner_right_arc = arc(center_outer_top_right, radius - border_width, M_PI / 2, M_PI / 4); // G <- H
		listed_point = ei_linked_point_join(upper_inner_right_arc, listed_point);

		// add E <- F <- G
		ei_linked_point_t *upper_outer_right_arc = arc(center_outer_top_right, radius, M_PI / 4, M_PI / 2); // E <- F
		listed_point = ei_linked_point_join(upper_outer_right_arc, listed_point);

		// add C <- D <- E
		ei_linked_point_t *upper_outer_left_arc = arc(center_outer_top_left, radius, M_PI / 2, M_PI); // E <- F
		listed_point = ei_linked_point_join(upper_outer_left_arc, listed_point);

		// add A <- B <- C
		ei_linked_point_t *bottom_outer_left_arc = arc(center_outer_bottom_left, radius, M_PI, 5 * M_PI / 4); // E <- F
		listed_point = ei_linked_point_join(bottom_outer_left_arc, listed_point);
		return listed_point;
	}
	else if (STRCMP(side, ==, "lower"))
	{ // create the lower side (A<-M<-N<-O<-P<-F<-G<-Q<-R<-S<-T<-L<-A)
		// add  T <- L <- A
		ei_linked_point_t *bottom_inner_left_arc = arc(center_outer_bottom_left, radius - border_width, 5 * M_PI / 4, 3 * M_PI / 2); // T <- L
		listed_point = ei_linked_point_join(bottom_inner_left_arc, listed_point);

		// add R <- S <- T
		ei_linked_point_t *bottom_inner_right_arc = arc(center_outer_bottom_right, radius - border_width, -M_PI / 2, 0); // R <- S
		listed_point = ei_linked_point_join(bottom_inner_right_arc, listed_point);

		// add G <- Q <- R
		ei_linked_point_t *upper_inner_right_arc = arc(center_outer_top_right, radius - border_width, 0, M_PI / 4); // G <- Q
		listed_point = ei_linked_point_join(upper_inner_right_arc, listed_point);

		// add P <- F <- G
		ei_linked_point_t *upper_outer_right_arc = arc(center_outer_top_right, radius, M_PI / 4, 0); // P <- F
		listed_point = ei_linked_point_join(upper_outer_right_arc, listed_point);

		// add N <- O <- P
		ei_linked_point_t *bottom_outer_right_arc = arc(center_outer_bottom_right, radius, 0, -M_PI / 2); // N <- O
		listed_point = ei_linked_point_join(bottom_outer_right_arc, listed_point);

		// add A <- M <- N
		ei_linked_point_t *bottom_outer_left_arc = arc(center_outer_bottom_left, radius, 3 * M_PI / 2, 5 * M_PI / 4); // A <- M
		listed_point = ei_linked_point_join(bottom_outer_left_arc, listed_point);

		return listed_point;
	}
	return NULL;
}

ei_linked_point_t *rounded_frame_plain(ei_rect_t rect,
									   int radius)
{
	int height = rect.size.height;
	int width = rect.size.width;
	int topleft_x = rect.top_left.x;
	int topleft_y = rect.top_left.y;
	ei_linked_point_t *listed_point = NULL;

	ei_point_t center_top_left = {topleft_x + radius, topleft_y + radius};
	ei_point_t center_bottom_right = {topleft_x + width - radius, topleft_y + height - radius};

	listed_point = arc(center_top_left, radius, M_PI, M_PI / 2);
	listed_point = ei_linked_point_join(arc((ei_point_t){center_bottom_right.x, center_top_left.y}, radius, M_PI / 2, 0), listed_point);
	listed_point = ei_linked_point_join(arc(center_bottom_right, radius, 0, -M_PI / 2), listed_point);
	listed_point = ei_linked_point_join(arc((ei_point_t){center_top_left.x, center_bottom_right.y}, radius, -M_PI / 2, -M_PI), listed_point);
	listed_point = ei_linked_point_add(listed_point, (ei_point_t){topleft_x, topleft_y + radius});

	return listed_point;
}

ei_linked_point_t *ei_linked_point_rectangle(ei_size_t size, ei_point_t top_left)
{
	ei_linked_point_t *points_rectangle = NULL;
	points_rectangle = ei_linked_point_add(points_rectangle, top_left);
	points_rectangle = ei_linked_point_add(points_rectangle, (ei_point_t){top_left.x + size.width, top_left.y});
	points_rectangle = ei_linked_point_add(points_rectangle, (ei_point_t){top_left.x + size.width, top_left.y + size.height});
	points_rectangle = ei_linked_point_add(points_rectangle, (ei_point_t){top_left.x, top_left.y + size.height});
	return points_rectangle;
}

ei_linked_point_t *ei_linked_point_square(int size, ei_point_t top_left)
{
	return ei_linked_point_rectangle((ei_size_t){size, size}, top_left);
}

ei_linked_point_t *ei_linked_point_diamond(int size, ei_point_t top_left)
{
	ei_linked_point_t *points_diamond = NULL;
	points_diamond = ei_linked_point_add(points_diamond, (ei_point_t){top_left.x + (int)size / 2, top_left.y});
	points_diamond = ei_linked_point_add(points_diamond, (ei_point_t){top_left.x, top_left.y + (int)size / 2});
	points_diamond = ei_linked_point_add(points_diamond, (ei_point_t){top_left.x + (int)size / 2, top_left.y + size});
	points_diamond = ei_linked_point_add(points_diamond, (ei_point_t){top_left.x + size, top_left.y + (int)size / 2});
	return points_diamond;
}

ei_linked_point_t *ei_linked_point_triangle(ei_size_t size, ei_point_t top_left)
{
	int base = size.width;
	int height = size.height;
	ei_linked_point_t *points_triangle = NULL;
	points_triangle = ei_linked_point_add(points_triangle, (ei_point_t){top_left.x + (int)base / 2, top_left.y});
	points_triangle = ei_linked_point_add(points_triangle, (ei_point_t){top_left.x, top_left.y + height});
	points_triangle = ei_linked_point_add(points_triangle, (ei_point_t){top_left.x + base, top_left.y + height});
	return points_triangle;
}

ei_point_t ei_get_coords_from_anchor(ei_anchor_t anchor, const ei_rect_t *container_rect, ei_size_t anchored_size)
{
	ei_size_t size = container_rect->size;
	int width = size.width - anchored_size.width;
	int height = size.height - anchored_size.height;

	switch (anchor)
	{
	case ei_anc_center:
		return ei_point_add(container_rect->top_left, ei_point((int)width / 2, (int)height / 2));
	case ei_anc_north:
		return ei_point_add(container_rect->top_left, ei_point((int)width / 2, 0));
	case ei_anc_northeast:
		return ei_point_add(container_rect->top_left, ei_point(width, 0));
	case ei_anc_east:
		return ei_point_add(container_rect->top_left, ei_point(width, (int)height / 2));
	case ei_anc_southeast:
		return ei_point_add(container_rect->top_left, ei_point(width, height));
	case ei_anc_south:
		return ei_point_add(container_rect->top_left, ei_point((int)width / 2, height));
	case ei_anc_southwest:
		return ei_point_add(container_rect->top_left, ei_point(0, height));
	case ei_anc_west:
		return ei_point_add(container_rect->top_left, ei_point(0, (int)height / 2));
	case ei_anc_northwest:
		return container_rect->top_left;
	default:
		return container_rect->top_left;
	}
}

ei_rect_t intersect_rect(const ei_rect_t *lhs, const ei_rect_t *rhs)
{
	// width
	ei_point_t rhs_bot_right = ei_point_add(rhs->top_left, ei_size_as_point(rhs->size));
	rhs_bot_right.x -= 1;
	rhs_bot_right.y -= 1;
	ei_point_t lhs_bot_right = ei_point_add(lhs->top_left, ei_size_as_point(lhs->size));
	lhs_bot_right.x -= 1;
	lhs_bot_right.y -= 1;
	ei_rect_t intersection = ei_rect_zero();

	intersection.top_left.x = MAX(lhs->top_left.x, rhs->top_left.x);
	intersection.top_left.y = MAX(lhs->top_left.y, rhs->top_left.y);

	ei_point_t intersection_bot_right;
	intersection_bot_right.x = MIN(rhs_bot_right.x, lhs_bot_right.x);
	intersection_bot_right.y = MIN(rhs_bot_right.y, lhs_bot_right.y);

	intersection.size.width = intersection_bot_right.x - intersection.top_left.x + 1;
	intersection.size.height = intersection_bot_right.y - intersection.top_left.y + 1;
	if (intersection.size.width < 0 || intersection.size.height < 0)
		return ei_rect_zero();

	return intersection;
}
