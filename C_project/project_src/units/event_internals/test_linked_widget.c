#include <assert.h>
#include "internals/event_internals/linked_widget.h"
#include "ei_event.h"
#include <stdio.h>
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "internals/ei_widgetclass_internal.h"
#include "helpers/ei_memory.h"
#include "internals/event_internals/event_internal.h"

static ei_widget_t *root_widget = NULL;

void test_ei_linked_widget_append();
void test_ei_linked_widget_remove();
void test_ei_linked_widget_register_callback();
void test_ei_linked_widget_unregister_callback();
void test_ei_linked_widget_find();

ei_bool_t print()
{
    printf("hello world\n");
    return EI_FALSE;
}

ei_bool_t print2()
{
    printf("ratatatata\n");
    return EI_FALSE;
}

int main()
{
    ei_frame_register_class();
    // create_event_array();
    ei_widgetclass_t *widgetclass = ei_widgetclass_from_name("frame");
    // set defaults value
    root_widget = widgetclass->allocfunc();
    widgetclass->setdefaultsfunc(root_widget);

    // set everything to 0
    root_widget->wclass = widgetclass;
    root_widget->parent = NULL;
    root_widget->children_head = NULL;
    root_widget->children_tail = NULL;
    root_widget->next_sibling = NULL;
    root_widget->user_data = NULL;
    root_widget->destructor = NULL;
    root_widget->pick_id = 0;
    root_widget->pick_color = (ei_color_t *)verified_calloc(1, sizeof(ei_color_t));
    root_widget->geom_params = NULL;
    root_widget->requested_size = (ei_size_t){0, 0};
    root_widget->screen_location = (ei_rect_t){{0, 0}, root_widget->requested_size};
    root_widget->content_rect = &root_widget->screen_location;

    test_ei_linked_widget_append();
    test_ei_linked_widget_remove();
    test_ei_linked_widget_register_callback();
    test_ei_linked_widget_unregister_callback();
    test_ei_linked_widget_find();
    ei_widget_destroy(root_widget);
    // destroy_event_array();
    ei_widgetclass_destroy(ei_widgetclass_get());
    return 0;
}

void test_ei_linked_widget_append()
{
    puts("\nTesting ei_linked_widget_append ...");

    {
        printf("Test 1: With 2 different widgets");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);

        ei_linked_widget_t *link = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = link;
        ei_linked_widget_t *to_append = ei_linked_widget_create(widget_bis);

        ei_linked_widget_t *inserted = ei_linked_widget_append(&link, to_append);

        assert(link->next == previous_head && "Test 1: The added element's next value should be the previous head address.\n");
        assert(link == to_append && "Test 1: The added element should be new head.\n");
        assert(inserted == to_append && "Test 1: The returned widget should be the one inserted !\n");
        puts("\tTest 1 OK.");
        ei_linked_widget_destroy(link);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
    }

    printf("Test 2: with 2 widgets sharing the same pick_id");
    ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
    ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
    widget_bis->pick_id = widget->pick_id;

    ei_linked_widget_t *link = ei_linked_widget_create(widget);
    ei_linked_widget_t *previous_head = link;
    ei_linked_widget_t *to_append = ei_linked_widget_create(widget_bis);
    ei_linked_widget_t *inserted = ei_linked_widget_append(&link, to_append);

    assert(link->next == NULL && "Test 2: No insertion should have occured.\n");
    assert(link == previous_head && "Test 2: The head should be unaltered.\n");
    assert(inserted == previous_head && "Test 2: The return value of ei_linked_widget_append should be the already inserted widget.\n");
    puts("\tTest 2 OK.");
    ei_linked_widget_destroy(to_append);
    ei_linked_widget_destroy(link);
    ei_widget_destroy(widget);
    ei_widget_destroy(widget_bis);

    puts("ei_linked_widget_append PASSED.");
}

void test_ei_linked_widget_remove()
{
    puts("\nTesting ei_linked_widget_remove ...");

    {
        printf("Test 1: remove when multiple items in list");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_remove(&head, widget_bis);
        assert(head->next == previous_head && "The element with widget 'widget_bis' was not correctly removed \n");
        puts("\tTest 1 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    printf("Test 2: removal of the head.");
    ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
    ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
    ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
    ei_linked_widget_t *head = ei_linked_widget_create(widget);
    ei_linked_widget_t *previous_head = head;
    ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
    ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
    ei_linked_widget_append(&head, second);
    ei_linked_widget_append(&head, third);

    ei_linked_widget_remove(&head, widget_3);
    assert(head == second && "Test 2: The head after should be the second element.\n");
    assert(head->next == previous_head && "Test 2: The head's next element is not what it should be.\n");
    puts("\tTest 2 OK");
    ei_linked_widget_destroy(head);
    ei_widget_destroy(widget);
    ei_widget_destroy(widget_bis);
    ei_widget_destroy(widget_3);

    puts("ei_linked_widget_remove PASSED");
}

void test_ei_linked_widget_register_callback()
{
    puts("\nTesting ei_linked_widget_register_callback ...");

    {
        printf("Test 1: Addition of a first callback in the middle of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_register_callback(second, print, NULL);

        assert(second->linked_callbacks->callback == print && "Test 1: The second widget first callback is not 'print'.\n");
        assert(previous_head->linked_callbacks == NULL && "Test 1: The last widget's callbacks should be empty.\n");
        assert(head->linked_callbacks == NULL && "Test 1: The head widget's callbacks should be empty.\n");
        puts("\tTest 1 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 2: Addition of two callbacks in the middle of the list, and a first at the end of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_register_callback(second, print2, NULL);
        ei_linked_widget_register_callback(second, print, NULL);

        ei_linked_widget_register_callback(previous_head, print, NULL);

        assert(second->linked_callbacks->callback == print && "Test 2: The second widget's first callback is not 'print'.\n");
        assert(second->linked_callbacks->next->callback == print2 && "Test 2: The second widget's second callback is not 'print2'.\n");
        assert(previous_head->linked_callbacks->callback == print && "Test 2: The last widget's first callback is not 'print'.\n");
        assert(head->linked_callbacks == NULL && "Test 2: The head widget's callbacks should be empty.\n");
        puts("\tTest 2 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    puts("ei_linked_widget_register_callback PASSED");
}

void test_ei_linked_widget_unregister_callback()
{
    puts("\nTesting ei_linked_widget_unregister_callback ...");

    {
        printf("Test 1: Addition and removal of a first callback in the middle of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_register_callback(second, print, NULL);
        ei_linked_widget_unregister_callback(second, print, NULL);

        assert(second->linked_callbacks == NULL && "Test 1: The second widget first callback is not NULL despite removal.\n");
        assert(previous_head->linked_callbacks == NULL && "Test 1: The last widget's callbacks should be empty.\n");
        assert(head->linked_callbacks == NULL && "Test 1: The head widget's callbacks should be empty.\n");
        puts("\tTest 1 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 2: Addition of two callbacks in the middle of the list, and a first at the end of the list. Removal of the second widget's second callback.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_register_callback(second, print2, NULL);
        ei_linked_widget_register_callback(second, print, NULL);

        ei_linked_widget_register_callback(previous_head, print, NULL);

        ei_linked_widget_unregister_callback(second, print2, NULL);

        assert(second->linked_callbacks->callback == print && "Test 2: The second widget's first callback should be unaltered and be 'print'.\n");
        assert(second->linked_callbacks->next == NULL && "Test 2: The second widget's second callback is not 'NULL' despite removal.\n");
        assert(previous_head->linked_callbacks->callback == print && "Test 2: The last widget's first callback is not 'print'.\n");
        assert(head->linked_callbacks == NULL && "Test 2: The head widget's callbacks should be empty.\n");
        puts("\tTest 2 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 3: Addition of two callbacks in the middle of the list, and a first at the end of the list. Removal of the second widget's second callback.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_register_callback(second, print2, NULL);
        ei_linked_widget_register_callback(second, print, NULL);

        ei_linked_widget_register_callback(previous_head, print, NULL);

        ei_linked_widget_unregister_callback(second, print, NULL);

        assert(second->linked_callbacks->callback == print2 && "Test 3: The second widget's first callback should have become 'print2'.\n");
        assert(second->linked_callbacks->next == NULL && "Test 3: The second widget's second callback should be 'NULL'.\n");
        assert(previous_head->linked_callbacks->callback == print && "Test 3: The last widget's first callback is not 'print'.\n");
        assert(head->linked_callbacks == NULL && "Test 3: The head widget's callbacks should be empty.\n");
        puts("\tTest 3 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    puts("ei_linked_widget_unregister_callback PASSED");
}

void test_ei_linked_widget_find()
{
    puts("\nTesting ei_linked_widget_find ...");

    {
        printf("Test 1: Search widget in the middle of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_t *found = ei_linked_widget_find(head, widget_bis);

        assert(found == second && "Test 1: Should have found the second link.\n");
        puts("\tTest 1 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 2: Search widget after removal in the middle of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_remove(&head, widget_bis);
        ei_linked_widget_t *found = ei_linked_widget_find(head, widget_bis);

        assert(found == NULL && "Test 2: After removal of a widget, a search upon it should return 'NULL'.\n");
        puts("\tTest 2 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 3: Search widget at the end of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_t *found = ei_linked_widget_find(head, widget);

        assert(found == previous_head && "Test 3: Should have found the third link.\n");
        puts("\tTest 3 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 4: Search widget at the beginning of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_widget_t *head = ei_linked_widget_create(widget);
        ei_linked_widget_t *previous_head = head;
        ei_linked_widget_t *second = ei_linked_widget_create(widget_bis);
        ei_linked_widget_t *third = ei_linked_widget_create(widget_3);
        ei_linked_widget_append(&head, second);
        ei_linked_widget_append(&head, third);

        ei_linked_widget_t *found = ei_linked_widget_find(head, widget_3);

        assert(found == third && "Test 4: Should have found the first link.\n");
        puts("\tTest 4 OK");
        ei_linked_widget_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }
    puts("ei_linked_widget_find PASSED");
}
