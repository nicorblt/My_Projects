/**
 * @file test_ei_draw.c
 * @author Eloi Navet, Benjamin Priour, Nicolas Roblet
 * @brief Module for testing ei_draw.h functions.
 * Testing list: ei_fill, ei_map_rgba, intersect_rect, ei_draw_text, ei_pick_color_from_pick_id
 * @version 0.1
 * @date 2022-05-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <assert.h>
#include <stdio.h>

#include "ei_draw.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "helpers/ei_geometry_helpers.h"
#include "internals/ei_picking_internal.h"

static ei_surface_t root_surface = NULL;

void test_ei_fill();
void test_ei_map_rgba();
void test_intersect_rect();
void test_ei_draw_text();
void test_offset_buffer_from_clipped();
void test_ei_copy_surface();
void test_ei_pick_color_from_pick_id();

int main()
{
    hw_init();
    ei_size_t window_size = {300, 300};
    root_surface = hw_create_window(window_size, EI_FALSE);
    test_ei_fill();     // OK
    test_ei_map_rgba(); // OK
    // test_ei_pick_color_from_pick_id();
    test_intersect_rect(); // OK
    test_ei_draw_text();
    // test_offset_buffer_from_clipped(); // OK
    test_ei_copy_surface(); // OK
    hw_quit();
    return (EXIT_SUCCESS);
}

/*
 * Unitary tests for the function \ref ei_fill.
 *
 */
void test_ei_fill()
{
    puts("Testing ei_fill ...");
    ei_size_t size;
    size.height = 128;
    size.width = 128;

    ei_color_t green = {0x00, 0xff, 0x00, 0xff};
    ei_surface_t surface = hw_surface_create(root_surface, size, EI_FALSE);

    printf("TEST 1 : without clipper, no alpha");
    hw_surface_lock(surface);
    ei_fill(surface, &green, NULL);
    uint32_t cooked_green = ei_map_rgba(surface, green);
    {
        uint32_t *pixel_it = (uint32_t *)hw_surface_get_buffer(surface);
        size_t nb_pixel = 128 * 128;
        for (size_t i = 0; i < nb_pixel; i++)
        {
            uint32_t pixel_color = 0;
            pixel_color = *(pixel_it++);

            if (pixel_color != cooked_green)
            {
                printf("pixel (%lu, %lu) with color %x\n", i % 128, i >> 7, pixel_color);
                assert(pixel_color == cooked_green && "Test 1: The color without clipper doesnt match in test_ei_fill!\n");
            }
        }
    }
    puts("\tOK");
    hw_surface_unlock(surface);

    printf("TEST 2 : With a clipper, no alpha.");
    {
        ei_rect_t clipper;
        clipper.size.height = 50;
        clipper.size.width = 128;
        clipper.top_left = ei_point(0, 10);
        ei_color_t blue = {0x00, 0x00, 0xff, 0xff};
        uint32_t cooked_blue = ei_map_rgba(surface, blue);

        hw_surface_lock(surface);
        uint32_t *pixel_it = (uint32_t *)hw_surface_get_buffer(surface);
        ei_fill(surface, &blue, &clipper);
        size_t nb_pixel = 128 * 128;
        for (size_t i = 0; i < nb_pixel; i++)
        {
            uint32_t pixel_color = 0;
            pixel_color = *(pixel_it++);

            if (((i >> 7) >= 10) && ((i >> 7) < 60)) // within clipper
            {
                if (pixel_color != cooked_blue)
                {
                    printf("Got pixel_color %x and cooked_blue %x\n", pixel_color, cooked_blue);
                    assert(pixel_color == cooked_blue && "Test 2: The color within the clipper doesn't match!\n");
                }
            }
            else // outside clipper
            {
                if (pixel_color != cooked_green)
                {
                    printf("Got pixel_color %x and cooked_green %x\n", pixel_color, cooked_green);
                    assert(pixel_color == cooked_green && "Test 2: Out of clipper color not preserved !\n");
                }
            }
        }
        puts("\tOK");
        hw_surface_unlock(surface);
        hw_surface_free(surface);
    }

    printf("TEST 3 : without clipper, alpha channel.");
    ei_surface_t surface_alpha = hw_surface_create(root_surface, size, EI_TRUE);
    ei_color_t red = {0xff, 0, 0, 0x33};
    hw_surface_lock(surface_alpha);
    ei_fill(surface_alpha, &red, NULL);
    uint32_t cooked_red = ei_map_rgba(surface_alpha, red);
    {
        uint32_t *pixel_it = (uint32_t *)hw_surface_get_buffer(surface_alpha);
        size_t nb_pixel = 128 * 128;
        for (size_t i = 0; i < nb_pixel; i++)
        {
            uint32_t pixel_color = 0;
            pixel_color = *(pixel_it++);

            if (pixel_color != cooked_red)
            {
                printf("pixel (%lu, %lu) with color %x\n", i % 128, i >> 7, pixel_color);
                assert(pixel_color == cooked_red && "TEST 3: The color without clipper doesnt match in test_ei_fill!\n");
            }
        }
    }
    puts("\tOK");
    hw_surface_unlock(surface_alpha);

    printf("TEST 4 : With a clipper, alpha channel.");
    ei_rect_t clipper;
    clipper.size.height = 50;
    clipper.size.width = 128;
    clipper.top_left = ei_point(0, 10);
    ei_color_t blue = {0x00, 0x00, 0xff, 0xcc};
    uint32_t cooked_blue = ei_map_rgba(surface_alpha, blue);

    hw_surface_lock(surface_alpha);
    uint32_t *pixel_it = (uint32_t *)hw_surface_get_buffer(surface_alpha);
    ei_fill(surface_alpha, &blue, &clipper);
    size_t nb_pixel = 128 * 128;
    for (size_t i = 0; i < nb_pixel; i++)
    {
        uint32_t pixel_color = 0;
        pixel_color = *(pixel_it++);

        if (((i >> 7) >= 10) && ((i >> 7) < 60)) // within clipper
        {
            if (pixel_color != cooked_blue)
            {
                printf("Got pixel_color %x and cooked_blue %x\n", pixel_color, cooked_blue);
                assert(pixel_color == cooked_blue && "TEST 4: The color within the clipper doesn't match!\n");
            }
        }
        else // outside clipper
        {
            if (pixel_color != cooked_red)
            {
                printf("Got pixel_color %x and cooked_red %x\n", pixel_color, cooked_red);
                assert(pixel_color == cooked_red && "TEST 4: Out of clipper color not preserved !\n");
            }
        }
    }
    puts("\tOK");
    hw_surface_unlock(surface_alpha);
    hw_surface_free(surface_alpha);

    puts("ei_fill OK.\n");
}

/*
 * Unitary tests for the function \ref ei_map_rgba.
 *
 */
void test_ei_map_rgba()
{
    puts("Testing ei_map_rgba...");
    ei_size_t size = {100, 100};

    ei_surface_t surface_no_alpha = hw_surface_create(root_surface, size, EI_FALSE);
    ei_surface_t surface_alpha = hw_surface_create(root_surface, size, EI_TRUE);
    hw_surface_lock(surface_no_alpha);
    hw_surface_lock(surface_alpha);

    ei_color_t color;
    int ir, ig, ib, ia;
    uint32_t res_map;

    printf("Test 1 : Test with a surface with no alpha channel.");
    color = (ei_color_t){5, 10, 32, 123};
    res_map = ei_map_rgba(surface_no_alpha, color);
    hw_surface_get_channel_indices(surface_no_alpha, &ir, &ig, &ib, &ia);
    ia = (ia == -1 && ((ib != 3) && (ig != 3) && (ir != 3))) ? 3 : -1;
    ia = (ia == -1 && ((ib != 2) && (ig != 2) && (ir != 2))) ? 2 : -1;
    ia = (ia == -1 && ((ib != 1) && (ig != 1) && (ir != 1))) ? 1 : -1;
    ia = (ia == -1 && ((ib != 0) && (ig != 0) && (ir != 0))) ? 0 : -1;
    assert((res_map >> (ir * 8) & 255) == (uint32_t)color.red && "Test 1 : red does not match.\n");
    assert((res_map >> (ig * 8) & 255) == (uint32_t)color.green && "Test 1 : green does not match.\n");
    assert((res_map >> (ib * 8) & 255) == (uint32_t)color.blue && "Test 1 : blue does not match.\n");
    assert((res_map >> (ia * 8) & 255) == (uint32_t)0xFF && "Test 1 : alpha should be 0xFF.\n");
    puts("\tOK");

    printf("Test 2 : Test with a surface with alpha channel");
    res_map = ei_map_rgba(surface_alpha, color);
    hw_surface_get_channel_indices(surface_alpha, &ir, &ig, &ib, &ia);
    assert((res_map >> (ir * 8) & 255) == (uint32_t)color.red && "Test 2 : red does not match.\n");
    assert((res_map >> (ig * 8) & 255) == (uint32_t)color.green && "Test 2 : green does not match.\n");
    assert((res_map >> (ib * 8) & 255) == (uint32_t)color.blue && "Test 2 : blue does not match.\n");
    assert((res_map >> (ia * 8) & 255) == (uint32_t)color.alpha && "Test 2 : alpha does not match.\n");
    puts("\tOK");

    printf("Test 3: Test with random colors with no alpha channel");
    color = (ei_color_t){rand() % 256, rand() % 256, rand() % 256, 123};
    res_map = ei_map_rgba(surface_no_alpha, color);
    hw_surface_get_channel_indices(surface_no_alpha, &ir, &ig, &ib, &ia);
    ia = (ia == -1 && ((ib != 3) && (ig != 3) && (ir != 3))) ? 3 : -1;
    ia = (ia == -1 && ((ib != 2) && (ig != 2) && (ir != 2))) ? 2 : -1;
    ia = (ia == -1 && ((ib != 1) && (ig != 1) && (ir != 1))) ? 1 : -1;
    ia = (ia == -1 && ((ib != 0) && (ig != 0) && (ir != 0))) ? 0 : -1;
    assert((res_map >> (ir * 8) & 255) == (uint32_t)color.red && "Test 3 : red does not match.\n");
    assert((res_map >> (ig * 8) & 255) == (uint32_t)color.green && "Test 3 : green does not match.\n");
    assert((res_map >> (ib * 8) & 255) == (uint32_t)color.blue && "Test 3 : blue does not match.\n");
    assert((res_map >> (ia * 8) & 255) == (uint32_t)0xFF && "Test 3 : alpha should be 0xFF.\n");
    puts("\tOK");

    printf("Test 4 : Test with random colors with alpha channel.");
    color = (ei_color_t){rand() % 256, rand() % 256, rand() % 256, rand() % 256};
    res_map = ei_map_rgba(surface_alpha, color);
    hw_surface_get_channel_indices(surface_alpha, &ir, &ig, &ib, &ia);
    assert((res_map >> (ir * 8) & 255) == (uint32_t)color.red && "Test 4: red does not match.\n");
    assert((res_map >> (ig * 8) & 255) == (uint32_t)color.green && "Test 4: green does not match.\n");
    assert((res_map >> (ib * 8) & 255) == (uint32_t)color.blue && "Test 4: blue does not match.\n");
    assert((res_map >> (ia * 8) & 255) == (uint32_t)color.alpha && "Test 4: alpha does not match.\n");
    puts("\tOK");

    // Unlock and update surface
    hw_surface_unlock(surface_alpha);
    hw_surface_unlock(surface_no_alpha);

    hw_surface_free(surface_alpha);
    hw_surface_free(surface_no_alpha);
    puts("ei_map_rgba OK.\n");
}

/*
 * Unitary test to see if \ref ei_pick_color_from_pick_id if the inverse function of \ref ei_map_rgba.
 *
 */
void test_ei_pick_color_from_pick_id()
{
    puts("Testing ei_pick_color_from_pick_id...");
    ei_size_t size = {100, 100};

    ei_surface_t surface_no_alpha = hw_surface_create(root_surface, size, EI_FALSE);
    ei_surface_t surface_alpha = hw_surface_create(root_surface, size, EI_TRUE);
    hw_surface_lock(surface_no_alpha);
    hw_surface_lock(surface_alpha);

    ei_color_t color;
    uint32_t res_map;
    ei_color_t *res_color;

    printf("Test 1 : Test with a surface with no alpha channel.");
    color = (ei_color_t){5, 10, 32, 123};
    res_map = ei_map_rgba(surface_no_alpha, color);
    res_color = ei_pick_color_from_pick_id(res_map);
    assert(res_color->red == color.red && "Test 1 : red does not match.\n");
    assert(res_color->green == color.green && "Test 1 : green does not match.\n");
    assert(res_color->blue == color.blue && "Test 1 : blue does not match.\n");
    assert(res_color->alpha == 0xFF && "Test 1 : alpha does not match.\n");
    puts("\tOK");

    printf("Test 2 : Test with a surface with an alpha channel.");
    color = (ei_color_t){5, 10, 32, 123};
    res_map = ei_map_rgba(surface_alpha, color);
    res_color = ei_pick_color_from_pick_id(res_map);
    assert(res_color->red == color.red && "Test 2 : red does not match.\n");
    assert(res_color->green == color.green && "Test 2 : green does not match.\n");
    assert(res_color->blue == color.blue && "Test 2 : blue does not match.\n");
    assert(res_color->alpha == color.alpha && "Test 2 : alpha does not match.\n");
    puts("\tOK");

    // Unlock and update surface
    hw_surface_unlock(surface_alpha);
    hw_surface_unlock(surface_no_alpha);

    hw_surface_free(surface_alpha);
    hw_surface_free(surface_no_alpha);
    puts("ei_pick_color_from_pick_id OK.\n");
}

/*
 * Unitary tests for the function \ref intersect_rect.
 *
 */
void test_intersect_rect()
{
    puts("Testing intersect_rect...");
    puts("Creating a test surface of size {100, 100}");
    ei_size_t size = {100, 100};
    ei_surface_t surface = hw_surface_create(root_surface, size, EI_TRUE);
    hw_surface_lock(surface);

    printf("Test 1 : clip the surface with a rectangle that is its size.");
    ei_rect_t clipper = {{0, 0}, size};
    const ei_rect_t surface_rect = hw_surface_get_rect(surface);

    ei_rect_t subsurface = intersect_rect(&surface_rect, &clipper);

    assert(subsurface.size.width == clipper.size.width && "Test 1 : width does not match.\n");
    assert(subsurface.size.height == clipper.size.height && "Test 1 : height does not match.\n");
    assert(subsurface.top_left.x == clipper.top_left.x && "Test 1 : top_left.x does not match.\n");
    assert(subsurface.top_left.y == clipper.top_left.y && "Test 1 : top_left.y does not match.\n");
    assert(EI_COMPARE_RECT(subsurface, ==, intersect_rect(&clipper, &surface_rect)) && "Test 1 : symmetry failed.\n");
    puts("\tOK");

    printf("Test 2 : clip the surface with a rectangle that is smaller than its size typically {50,75} and clipper.top_left = (0,0).");
    clipper.size = (ei_size_t){50, 75};

    subsurface = intersect_rect(&surface_rect, &clipper);

    assert(subsurface.size.width == clipper.size.width && "Test 2 : width does not match.\n");
    assert(subsurface.size.height == clipper.size.height && "Test 2 : height does not match.\n");
    assert(subsurface.top_left.x == clipper.top_left.x && "Test 2 : top_left.x does not match.\n");
    assert(subsurface.top_left.y == clipper.top_left.y && "Test 2 : top_left.y does not match.\n");
    assert(EI_COMPARE_RECT(subsurface, ==, intersect_rect(&clipper, &surface_rect)) && "Test 2 : symmetry failed.\n");
    puts("\tOK");

    printf("Test 3 : clip the surface with a rectangle that is smaller than its size typically {50,75} but clipper.top_left = (10,10).");
    clipper.top_left.x = clipper.top_left.y = 10;

    subsurface = intersect_rect(&surface_rect, &clipper);

    assert(subsurface.size.width == clipper.size.width && "Test 3 : width does not match.\n");
    assert(subsurface.size.height == clipper.size.height && "Test 3 : height does not match.\n");
    assert(subsurface.top_left.x == clipper.top_left.x && "Test 3 : top_left.x does not match.\n");
    assert(subsurface.top_left.y == clipper.top_left.y && "Test 3 : top_left.y does not match.\n");
    assert(EI_COMPARE_RECT(subsurface, ==, intersect_rect(&clipper, &surface_rect)) && "Test 3 : symmetry failed.\n");
    puts("\tOK");

    printf("Test 4 : clip the surface with a rectangle that contains the surface.");
    clipper.size.width = 150;
    clipper.size.height = 175;
    clipper.top_left.x = clipper.top_left.y = -10;

    subsurface = intersect_rect(&surface_rect, &clipper);

    assert(subsurface.size.width == size.width && "Test 4 : width does not match.\n");
    assert(subsurface.size.height == size.height && "Test 4 : height does not match.\n");
    assert(subsurface.top_left.x == 0 && "Test 4 : top_left.x does not match.\n");
    assert(subsurface.top_left.y == 0 && "Test 4 : top_left.y does not match.\n");
    assert(EI_COMPARE_RECT(subsurface, ==, intersect_rect(&clipper, &surface_rect)) && "Test 4 : symmetry failed.\n");
    puts("\tOK");

    printf("Test 5 : clip the surface with a clip surface which is outside of the surface.");
    clipper.size.width = 10;
    clipper.size.height = 10;
    clipper.top_left.x = clipper.top_left.y = -20;

    subsurface = intersect_rect(&surface_rect, &clipper);

    assert(subsurface.size.width == 0 && "Test 5 : width does not match.\n");
    assert(subsurface.size.height == 0 && "Test 5 : height does not match.\n");
    assert(subsurface.top_left.x == 0 && "Test 5 : top_left.x does not match.\n");
    assert(subsurface.top_left.y == 0 && "Test 5 : top_left.y does not match.\n");
    assert(EI_COMPARE_RECT(subsurface, ==, intersect_rect(&clipper, &surface_rect)) && "Test 5 : symmetry failed.\n");
    puts("\tOK");

    printf("Test 6 : the widget is integrally inside the clipping surface.");
    clipper.size.width = 160;
    clipper.size.height = 160;
    clipper.top_left.x = clipper.top_left.y = -10;

    subsurface = intersect_rect(&surface_rect, &clipper);

    assert(subsurface.size.width == size.width && "Test 6 : width does not match.\n");
    assert(subsurface.size.height == size.height && "Test 6 : height does not match.\n");
    assert(subsurface.top_left.x == 0 && "Test 6 : top_left.x does not match.\n");
    assert(subsurface.top_left.y == 0 && "Test 6 : top_left.y does not match.\n");
    assert(EI_COMPARE_RECT(subsurface, ==, intersect_rect(&clipper, &surface_rect)) && "Test 6 : symmetry failed.\n");
    puts("\tOK");

    hw_surface_unlock(surface);
    hw_surface_free(surface);
    puts("intersect_rect OK.\n");
}

/*
 * Unitary tests for the function \ref offset_buffer_from_clipped.
 *
 */
void test_offset_buffer_from_clipped()
{
    puts("Testing offset_buffer_from_clipped...");
    ei_size_t surface_size = {100, 100};
    ei_surface_t surface = hw_surface_create(root_surface, surface_size, EI_TRUE);
    ei_rect_t clipper;
    clipper.size.height = 50;
    clipper.size.width = 128;
    clipper.top_left = ei_point(0, 10);
    hw_surface_lock(surface);

    const ei_rect_t surface_rect = hw_surface_get_rect(surface);
    ei_rect_t clipped = intersect_rect(&surface_rect, &clipper);

    uint32_t *it = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t *first_pix_clipped = offset_buffer_from_clipped(surface, &clipped);

    size_t expected_pos = (size_t)surface_size.width * clipped.top_left.y + clipped.top_left.x;
    for (size_t pos = 0; pos != expected_pos; pos++, it++)
        ;
    assert(it == first_pix_clipped && "Clipped first pixel is not at the good offset.\n");

    hw_surface_unlock(surface);
    hw_surface_free(surface);
    puts("offset_buffer_from_clipped OK.\n");
}

/*
 * Unitary tests for the function \ref ei_draw_text.
 *
 */
void test_ei_draw_text()
{
    puts("Testing ei_draw_text...");
    puts("Test not found\n");
}

/*
 * Unitary tests for the function \ref ei_copy_surface.
 *
 */
void test_ei_copy_surface()
{
    puts("Testing ei_copy_surface...");
    printf("TEST 1: with dst_rect, src_rect both NULL and alpha false.");
    {
        ei_size_t size;
        size.height = 128;
        size.width = 128;

        ei_color_t green = {0x00, 0xff, 0x00, 0xff};
        ei_color_t red = {0x00, 0x00, 0xff, 0xff};

        ei_surface_t source = hw_surface_create(root_surface, size, EI_FALSE);      // pas certain des arguments
        ei_surface_t destination = hw_surface_create(root_surface, size, EI_FALSE); // pas certain des arguments

        hw_surface_lock(source);
        hw_surface_lock(destination);
        ei_fill(source, &green, NULL);
        ei_fill(destination, &red, NULL);
        ei_copy_surface(destination, NULL, source, NULL, EI_FALSE);
        uint32_t cooked_green = ei_map_rgba(destination, green);

        {
            uint32_t *pixel_it = (uint32_t *)hw_surface_get_buffer(destination);
            size_t nb_pixel = 128 * 128;
            for (size_t i = 0; i < nb_pixel; i++)
            {
                uint32_t pixel_color = 0;
                pixel_color = *(pixel_it++);

                if (pixel_color != cooked_green)
                {
                    assert(pixel_color == cooked_green && "The color with dst_rect and src_rect NULL doesnt match in test_copy_surface!\n");
                }
            }
        }
        hw_surface_unlock(source);
        hw_surface_unlock(destination);
        hw_surface_free(source);
        hw_surface_free(destination);
    }
    puts("\tOK");

    printf("TEST 2: with dst_rect, src_rect both not NULL and alpha false.");
    {
        ei_size_t size;
        size.height = 128;
        size.width = 128;

        ei_color_t green = {0x00, 0xff, 0x00, 0xff};
        ei_color_t red = {0xff, 0x00, 0x00, 0xff};
        ei_color_t blue = {0x00, 0x00, 0xff, 0xff};

        ei_surface_t source = hw_surface_create(root_surface, size, EI_FALSE);
        ei_surface_t destination = hw_surface_create(root_surface, size, EI_FALSE);
        hw_surface_lock(source);
        hw_surface_lock(destination);

        ei_fill(source, &green, NULL);
        ei_fill(destination, &red, NULL);
        ei_rect_t dst_rect;
        dst_rect.size.width = 128;
        dst_rect.size.height = 64;
        dst_rect.top_left = ei_point(0, 64);

        ei_rect_t src_rect;
        src_rect.size.width = 128;
        src_rect.size.height = 64;
        src_rect.top_left = ei_point(0, 42);
        ei_fill(source, &blue, &src_rect);

        ei_copy_surface(destination, &dst_rect, source, &src_rect, EI_FALSE);
        uint32_t cooked_green = ei_map_rgba(destination, green);
        uint32_t cooked_red = ei_map_rgba(destination, red);
        uint32_t cooked_blue = ei_map_rgba(destination, blue);

        {
            uint32_t *pixel_it = (uint32_t *)hw_surface_get_buffer(destination);
            size_t nb_pixel = size.height * size.width;

            for (size_t i = 0; i < nb_pixel; i++)
            {
                uint32_t pixel_color = 0;
                pixel_color = *(pixel_it++);

                if (ei_point_is_in_rect(ei_point(i % 128, i >> 7), dst_rect)) // if within the area of destination copied from source, should have blue
                {
                    if (pixel_color != cooked_blue)
                    {
                        printf("\nrow %lu, column %lu, color obtained %x\n", i >> 7, i % 128, pixel_color);
                        printf("cooked_red: %x, cooked_blue:%x, cooked_green:%x\n", cooked_red, cooked_blue, cooked_green);
                        assert(pixel_color == cooked_blue && "The color inside the destination and with dst_rect and src_rect not NULL doesnt match in test_copy_surface!\n");
                    }
                }
                else
                {
                    if (pixel_color != cooked_red)
                    {
                        printf("\nrow %lu, column %lu, color obtained %x\n", i >> 7, i % 128, pixel_color);
                        printf("cooked_red: %x, cooked_blue:%x, cooked_green:%x\n", cooked_red, cooked_blue, cooked_green);
                        assert(pixel_color == cooked_red && "The color outside the destination of rect and with dst_rect and src_rect not NULL doesnt match in test_copy_surface!\n");
                    }
                }
            }
        }
        hw_surface_unlock(source);
        hw_surface_unlock(destination);
        hw_surface_free(source);
        hw_surface_free(destination);
    }
    puts("\tOK");
    puts("ei_copy_surface OK\n");
}