#include <assert.h>
#include <stdio.h>

#include "internals/event_internals/linked_widget.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "internals/ei_widgetclass_internal.h"
#include "helpers/ei_memory.h"
#include "internals/event_internals/event_internal.h"
#include "ei_application.h"

void test_can_merge_rect();
void test_ei_merge_rect();
void test_ei_merge_invalidate_rect();

int main()
{
    test_can_merge_rect();
    test_ei_merge_rect();
    test_ei_merge_invalidate_rect();
    return (EXIT_SUCCESS);
}

void test_can_merge_rect()
{
    puts("Testing can_merge_rect...");

    {
        printf("Test 1 : distinct rectangles.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {200, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_bool_t can_merge = ei_can_merge_rect(rect1, rect2);
        assert(can_merge == EI_FALSE && "Test 1 failed !");
        puts("\t OK");
    }

    {
        printf("Test 2 : identical rectangles.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {100, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_bool_t can_merge = ei_can_merge_rect(rect1, rect2);
        assert(can_merge == EI_TRUE && "Test 2 failed !");
        puts("\t OK");
    }

    {
        printf("Test 3 : rectangles which have less than 50 percent area in commun.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {160, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_bool_t can_merge = ei_can_merge_rect(rect1, rect2);
        assert(can_merge == EI_FALSE && "Test 3 failed !");
        puts("\t OK");
    }

    {
        printf("Test 4 : rectangles which have more than 50 percent area in commun.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {140, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_bool_t can_merge = ei_can_merge_rect(rect1, rect2);
        assert(can_merge == EI_TRUE && "Test 4 failed !");
        puts("\t OK");
    }

    puts("can_merge_rect OK.");
}

void test_ei_merge_rect()
{
    puts("Testing ei_merge_rect...");

    {
        printf("Test 1 : distinct rectangles.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {200, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_rect_t merged_rect = ei_merge_rect(rect1, rect2);
        assert(merged_rect.top_left.x == 100 && "Test 1 failed : wrong top_left.x value");
        assert(merged_rect.top_left.y == 100 && "Test 1 failed : wrong top_left.y value");
        assert(merged_rect.size.width == 200 && "Test 1 failed : wrong size.width value");
        assert(merged_rect.size.height == 100 && "Test 1 failed : wrong size.height value");
        puts("\t OK");
    }

    {
        printf("Test 2 : identical rectangles.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {100, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_rect_t merged_rect = ei_merge_rect(rect1, rect2);
        assert(merged_rect.top_left.x == 100 && "Test 2 failed : wrong top_left.x value");
        assert(merged_rect.top_left.y == 100 && "Test 2 failed : wrong top_left.y value");
        assert(merged_rect.size.width == 100 && "Test 2 failed : wrong size.width value");
        assert(merged_rect.size.height == 100 && "Test 2 failed : wrong size.height value");
        puts("\t OK");
    }

    {
        printf("Test 3 : rectangles which have less than 50 percent area in commun.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {160, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_rect_t merged_rect = ei_merge_rect(rect1, rect2);
        assert(merged_rect.top_left.x == 100 && "Test 3 failed : wrong top_left.x value");
        assert(merged_rect.top_left.y == 100 && "Test 3 failed : wrong top_left.y value");
        assert(merged_rect.size.width == 160 && "Test 3 failed : wrong size.width value");
        assert(merged_rect.size.height == 100 && "Test 3 failed : wrong size.height value");
        puts("\t OK");
    }

    {
        printf("Test 4 : rectangles which have more than 50 percent area in commun.");
        ei_point_t rect1_origin = {100, 100};
        ei_point_t rect2_origin = {140, 100};
        ei_size_t rect1_size = {100, 100};
        ei_size_t rect2_size = {100, 100};
        ei_rect_t rect1 = {rect1_origin, rect1_size};
        ei_rect_t rect2 = {rect2_origin, rect2_size};
        ei_rect_t merged_rect = ei_merge_rect(rect1, rect2);
        assert(merged_rect.top_left.x == 100 && "Test 4 failed : wrong top_left.x value");
        assert(merged_rect.top_left.y == 100 && "Test 4 failed : wrong top_left.y value");
        assert(merged_rect.size.width == 140 && "Test 4 failed : wrong size.width value");
        assert(merged_rect.size.height == 100 && "Test 4 failed : wrong size.height value");
        puts("\t OK");
    }

    puts("ei_merge_rect OK.");
}

void test_ei_merge_invalidate_rect()
{
    puts("Testing ei_merge_invalidate_rect...");

    puts("ei_merge_invalidate_rect OK.");
}