/**
 * @file test_ei_geometry_helpers.c
 * @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 * @brief Module for testing ei_geometrymanager.h functions.
 * Testing list: /
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "ei_types.h"
#include "ei_event.h"
#include "ei_utils.h"
#include "helpers/ei_geometry_helpers.h"
#include "hw_interface.h"
#include "ei_draw.h"

static ei_surface_t root_surface = NULL;

// Debug use
static void print_linked_list(ei_linked_point_t *list)
{
    ei_linked_point_t *current = list;
    while (current != NULL)
    {
        printf("(%d,%d) -> ", current->point.x, current->point.y);
        current = current->next;
    }
    printf("NULL\n");
}

void test_ei_linked_point_add();
void test_ei_linked_point_join();
void test_free_linked_list();
void test_arc();
void test_rounded_frame();
void test_rounded_frame_borders();
void test_rounded_frame_plain();
void test_ei_linked_point_rectangle();
void test_ei_linked_point_square();
void test_ei_linked_point_diamond();
void test_ei_linked_point_triangle();
void test_ei_get_coords_from_anchor();
void test_intersect_rect();

int main()
{
    // Non graphical functions
    // test_ei_linked_point_add(); // OK
    // test_ei_linked_point_join(); // OK
    // test_free_linked_list(); // OK

    // Graphical functions
    hw_init();
    ei_size_t window_size = {600, 600};
    root_surface = hw_create_window(window_size, EI_FALSE);
    ei_color_t color = {255, 255, 255, 128};
    // ei_fill(root_surface, &color, NULL);
    // test_arc(); // OK
    // test_rounded_frame(); // OK
    // test_rounded_frame_borders(); // OK
    // test_intersect_rect(); // OK
    // test_rounded_frame_plain(); // OK
    // test_ei_linked_point_rectangle(); // OK
    // test_ei_linked_point_square(); // OK
    // test_ei_linked_point_diamond(); // OK
    // test_ei_linked_point_triangle(); // OK
    test_ei_get_coords_from_anchor();
    // test_intersect_rect();
    hw_quit();

    return (EXIT_SUCCESS);
}

/*
 * Unitary tests for the function \ref ei_linked_point_add.
 *
 */
void test_ei_linked_point_add()
{
    ei_linked_point_t *list = NULL;
    ei_point_t point = {0, 0};
    ei_linked_point_t *list_new = ei_linked_point_add(list, point);
    assert(list_new != NULL && "List_new is NULL");
    assert(list_new->point.x == 0 && "List_new->point.x is not 0");
    assert(list_new->point.y == 0 && "List_new->point.y is not 0");
    assert(list_new->next == NULL && "List_new->next is not NULL");
}

/*
 * Unitary tests for the function \ref ei_linked_point_join.
 *
 */
void test_ei_linked_point_join()
{
    ei_linked_point_t *list_1 = NULL;
    ei_linked_point_t *list_2 = NULL;
    list_1 = ei_linked_point_add(list_1, (ei_point_t){0, 0});               // l1 = (0,0)
    list_1 = ei_linked_point_add(list_1, (ei_point_t){0, 1});               // l1 = (0,1) -> (0,0)
    list_2 = ei_linked_point_add(list_2, (ei_point_t){1, 1});               // l2 = (1,1)
    ei_linked_point_t *list_jointed = ei_linked_point_join(list_1, list_2); // lj = (0,1) -> (0,0) -> (1,1)

    assert(list_jointed != NULL && "List_jointed is NULL");
    assert(list_jointed->point.x == 0 && "List_jointed->point.x is not 0");
    assert(list_jointed->point.y == 1 && "List_jointed->point.y is not 1");
    assert(list_jointed->next != NULL && "List_jointed->next is NULL");
    assert(list_jointed->next->point.x == 0 && "List_jointed->next->point.x is not 0");
    assert(list_jointed->next->point.y == 0 && "List_jointed->next->point.y is not 0");
    assert(list_jointed->next->next != NULL && "List_jointed->next->next is NULL");
    assert(list_jointed->next->next->point.x == 1 && "List_jointed->next->next->point.x is not 1");
    assert(list_jointed->next->next->point.y == 1 && "List_jointed->next->next->point.y is not 1");
    assert(list_jointed->next->next->next == NULL && "List_jointed->next->next->next is not NULL");
}

/*
 * Unitary tests for the function \ref free_linked_list.
 *
 */
void test_free_linked_list()
{
    ei_linked_point_t *list = NULL;
    ei_linked_point_add(list, (ei_point_t){0, 0});
    ei_linked_point_add(list, (ei_point_t){0, 1});
    ei_linked_point_add(list, (ei_point_t){1, 1});
    free_linked_list(list);
    assert(list == NULL && "Freed list is not NULL");
}

/*
 * Unitary tests for the function \ref arc.
 *
 */
void test_arc()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 600;
    clipper.top_left = ei_point(0, 0);

    ei_point_t center = {300, 300};
    uint32_t radius = 120;
    ei_color_t color = {0, 0, 0, 128};

    hw_surface_lock(root_surface);

    // First arc (red)
    ei_linked_point_t *points_arc = arc(center, radius, 0.0, M_PI / 2);
    color.red = 0xff;
    ei_draw_polyline(root_surface, points_arc, color, &clipper);

    // Second arc (blue)
    ei_linked_point_t *points_arc2 = arc(center, radius, M_PI / 2, 9 * M_PI);
    color.red = 0;
    color.blue = 0xff;
    points_arc = ei_linked_point_join(points_arc2, points_arc);
    ei_draw_polyline(root_surface, points_arc, color, &clipper);

    // No arc (brown)
    points_arc = arc(center, radius, M_PI, M_PI);
    color.blue = 0;
    color.red = 128;
    ei_draw_polyline(root_surface, points_arc, color, &clipper);

    // Third arc (green)
    points_arc = arc(center, radius, 3 * M_PI / 2, M_PI);
    color.red = 0;
    color.green = 0xff;
    ei_draw_polyline(root_surface, points_arc, color, &clipper);

    // Fourth arc (cyan)
    points_arc = arc((ei_point_t){center.x + radius, center.y}, radius, 0, 2 * M_PI); // => entire circle
    color.blue = 0xff;
    ei_draw_polyline(root_surface, points_arc, color, &clipper);

    puts("The figure should be a partial circle with 3 arcs (red, blue, green) and a cyan circle.");

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref rounded_frame.
 *
 */
void test_rounded_frame()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 400;
    clipper.top_left = ei_point(100, 100);
    ei_rect_t frame;
    frame.size.height = 399;
    frame.size.width = 399;
    frame.top_left = ei_point(100, 100);
    ei_color_t color_top = {0xff, 0xff, 0xff, 0xff};
    ei_color_t color_bottom = {0xff, 0x00, 0xff, 0xff};
    ei_color_t color_all = {0x00, 0x00, 0xff, 0xff};

    uint32_t radius = 40;

    hw_surface_lock(root_surface);

    ei_linked_point_t *points = rounded_frame(frame, radius, "upper");
    ei_draw_polyline(root_surface, points, color_top, &clipper);
    ei_linked_point_t *points2 = rounded_frame(frame, radius, "lower");
    ei_draw_polyline(root_surface, points2, color_bottom, &clipper);
    ei_linked_point_t *points3 = rounded_frame(frame, radius, "all");
    ei_draw_polyline(root_surface, points3, color_all, &clipper);

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref rounded_frame_borders.
 *
 */
void test_rounded_frame_borders()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 400;
    clipper.top_left = ei_point(100, 100);
    ei_rect_t frame;
    frame.size.height = frame.size.width = 399;
    frame.top_left = ei_point(100, 100);
    ei_color_t color_top = {0xff, 0xff, 0xff, 0xff};
    ei_color_t color_bottom = {0xff, 0x00, 0xff, 0xff};

    uint32_t radius = 40;
    uint32_t border_width = 20;

    hw_surface_lock(root_surface);

    ei_linked_point_t *points = rounded_frame_borders(frame, radius, "upper", border_width);
    ei_draw_polyline(root_surface, points, color_top, &clipper);
    ei_linked_point_t *points2 = rounded_frame_borders(frame, radius, "lower", border_width);
    ei_draw_polyline(root_surface, points2, color_bottom, &clipper);

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref rounded_frame_plain.
 *
 */
void test_rounded_frame_plain()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 400;
    clipper.top_left = ei_point(100, 100);
    ei_rect_t frame;
    frame.size.height = frame.size.width = 399;
    frame.top_left = ei_point(100, 100);
    ei_color_t color = {0xff, 0xff, 0xff, 0xff};

    uint32_t radius = 40;

    hw_surface_lock(root_surface);

    ei_linked_point_t *points = rounded_frame_plain(frame, radius);
    ei_draw_polyline(root_surface, points, color, &clipper);

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref ei_linked_point_rectangle.
 *
 */
void test_ei_linked_point_rectangle()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 400;
    clipper.top_left = ei_point(100, 200);
    ei_rect_t frame;
    frame.size.height = frame.size.width = 399;
    frame.top_left = ei_point(100, 200);
    ei_color_t color = {0xbb, 0xff, 0xff, 0xff};

    ei_size_t size = {100, 50};

    hw_surface_lock(root_surface);

    ei_linked_point_t *points = ei_linked_point_rectangle(size, frame.top_left);
    ei_draw_polygon(root_surface, points, color, &clipper);

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref ei_linked_point_square.
 *
 */
void test_ei_linked_point_square()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 400;
    clipper.top_left = ei_point(200, 100);
    ei_rect_t frame;
    frame.size.height = frame.size.width = 399;
    frame.top_left = ei_point(200, 100);
    ei_color_t color = {0xff, 0xaa, 0xff, 0xff};

    int size = 100;

    hw_surface_lock(root_surface);

    ei_linked_point_t *points = ei_linked_point_square(size, frame.top_left);
    ei_draw_polygon(root_surface, points, color, &clipper);

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref ei_linked_point_diamond.
 *
 */
void test_ei_linked_point_diamond()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 400;
    clipper.top_left = ei_point(200, 200);
    ei_rect_t frame;
    frame.size.height = frame.size.width = 399;
    frame.top_left = ei_point(200, 200);
    ei_color_t color = {0xaa, 0xff, 0xff, 0xff};

    int size = 100;

    hw_surface_lock(root_surface);

    ei_linked_point_t *points = ei_linked_point_diamond(size, frame.top_left);
    ei_draw_polygon(root_surface, points, color, &clipper);

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref ei_linked_point_triangle.
 *
 */
void test_ei_linked_point_triangle()
{
    ei_event_t event;
    ei_rect_t clipper;
    clipper.size.height = clipper.size.width = 400;
    clipper.top_left = ei_point(250, 250);
    ei_rect_t frame;
    frame.size.height = frame.size.width = 399;
    frame.top_left = ei_point(250, 250);
    ei_color_t color = {0xff, 0xff, 0xaa, 0xff};

    ei_size_t size = {100, 50};

    hw_surface_lock(root_surface);

    ei_linked_point_t *points = ei_linked_point_triangle(size, frame.top_left);
    ei_draw_polygon(root_surface, points, color, &clipper);

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
}

/*
 * Unitary tests for the function \ref ei_get_coord_from_anchor.
 *
 */
void test_ei_get_coords_from_anchor()
{
    puts("Testing ei_get_coords_from_anchor ...");
    ei_anchor_t anchor = ei_anc_center;
    ei_size_t size = {100, 50};
    ei_point_t point = {100, 200};

    printf("Coords should always be : (%d,%d).\n", point.x, point.y);

    ei_rect_t rect = {point, size};

    ei_point_t coord;

    hw_surface_lock(root_surface);

    printf("Test 1 : anchor on center");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_center, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 2 : anchor on north");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_north, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 3 : anchor on northwest");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_northwest, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 4 : anchor on west");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_west, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 5 : anchor on southwest");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_southwest, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 6 : anchor on south");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_south, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 7 : anchor on southeast");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_southeast, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 8 : anchor on east");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_east, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    printf("Test 9 : anchor on northeast");
    coord = ei_get_coords_from_anchor((ei_anchor_t)ei_anc_northeast, &rect, size);
    assert(coord.x == point.x && "Wrong x coord\n");
    assert(coord.y == point.y && "Wrong y coord\n");
    puts("\tOK");

    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, NULL);

    puts("ei_get_coords_from_anchor OK.\n");
}

/*
 * Unitary tests for the function \ref intersect_rect.
 *
 */
void test_intersect_rect()
{
}
