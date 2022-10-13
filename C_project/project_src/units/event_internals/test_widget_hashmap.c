#include <assert.h>
#include "internals/event_internals/widget_hashmap.h"
#include "ei_event.h"
#include <stdio.h>
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "internals/ei_widgetclass_internal.h"
#include "helpers/ei_memory.h"
#include "internals/event_internals/event_internal.h"

static ei_widget_t *root_widget = NULL;

void test_ei_hashmap_widget_insert();
void test_ei_hashmap_widget_remove();
void test_ei_hashmap_widget_find();

// tester resize et refresh

ei_bool_t print()
{
    printf("Engagé Baleine!\n");
    return EI_FALSE;
}

ei_bool_t print2()
{
    printf("tu ne m'as pas l'air très taureau sur les bords.\n");
    return EI_FALSE;
}

ei_bool_t print3()
{
    printf("Est-ce que c'est toi John Wayne, où est-ce que c'est moi ?\n");
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

    printf("sizeof %lu", sizeof(ei_linked_widget_t));
    test_ei_hashmap_widget_insert();
    // test_ei_hashmap_widget_remove();
    // test_ei_hashmap_widget_find();
    ei_widget_destroy(root_widget);
    // destroy_event_array();
    ei_widgetclass_destroy(ei_widgetclass_get());
    return 0;
}

void test_ei_hashmap_widget_insert()
{
    puts("\nTesting ei_hashmap_widget_insert ...");

    {
        printf("Test 1: Insert 3 different widgets.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL); // pick_id 1
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

        widget_bis->pick_id = 11;
        widget_3->pick_id = 21;

        ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);

        ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print, NULL);
        ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

        // Dont forget to edit the hash function to always return 0 before launching this test.
        ei_linked_widget_t *head = ei_hashmap_widget_get_page(hashmap, 1);
        assert(hashmap->length == 3 && "Test 1: Length of the hashmap should be 3.\n");
        assert(head->widget->pick_id == widget_3->pick_id && "Test 1: Head of the linked list should be 'widget_3'.\n");
        assert(head->linked_callbacks->callback == print2 && "Test 1: Head of the linked list should have callback 'print2' registered.\n");
        assert(head->next->widget->pick_id == widget_bis->pick_id && "Test 1: Second of the linked list should be 'widget_bis'.\n");
        assert(head->next->linked_callbacks->callback == print && "Test 1: Second of the linked list should have callback 'print' registered.\n");
        assert(head->next->next->widget->pick_id == widget->pick_id && "Test 1: Third of the linked list should be 'widget'.\n");
        assert(head->next->next->linked_callbacks == NULL && "Test 1: Third of the linked list should have no callback registered.\n");
        assert(head->next->next->next == NULL && "Test 1: Fourth of the linked list should be 'NULL'.\n");
        puts("\tTest 1 OK.");
        ei_hashmap_widget_destroy(hashmap);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    printf("Test 2: Insert the same widgets twice, but with different callbacks.");
    ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL); // pick_id 4
    ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
    ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

    ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);
    widget_bis->pick_id = 14;
    widget_3->pick_id = 24;

    ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
    ei_hashmap_widget_insert(hashmap, widget_bis, print2, NULL);
    ei_hashmap_widget_insert(hashmap, widget_bis, print, NULL);
    ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

    ei_linked_widget_t *head = ei_hashmap_widget_get_page(hashmap, 4);
    assert(hashmap->length == 3 && "Test 2: Length of the hashmap should be 3.\n");
    assert(head->widget->pick_id == widget_3->pick_id && "Test 2: Head of the linked list should be 'widget_3'.\n");
    assert(head->linked_callbacks->callback == print2 && "Test 2: Head of the linked list should have callback 'print2' registered.\n");
    assert(head->next->widget->pick_id == widget_bis->pick_id && "Test 2: Second of the linked list should be 'widget_bis'.\n");
    assert(head->next->linked_callbacks->callback == print && "Test 2: Second of the linked list should have callback 'print' registered first.\n");
    assert(head->next->linked_callbacks->next->callback == print2 && "Test 2: Second of the linked list should have callback 'print2' registered second.\n");
    assert(head->next->next->widget->pick_id == widget->pick_id && "Test 2: Third of the linked list should be 'widget'.\n");
    assert(head->next->next->linked_callbacks == NULL && "Test 2: Third of the linked list should have no callback registered.\n");
    assert(head->next->next->next == NULL && "Test 2: Fourth of the linked list should be 'NULL'.\n");
    puts("\tTest 2 OK.");
    ei_hashmap_widget_destroy(hashmap);
    ei_widget_destroy(widget);
    ei_widget_destroy(widget_bis);
    ei_widget_destroy(widget_3);

    puts("ei_hashmap_widget_insert PASSED.");
}

void test_ei_hashmap_widget_remove()
{
    puts("\nTesting ei_hashmap_widget_remove ...");

    {
        printf("Test 1: remove a widget and all its callback when multiple items in list");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL); // pick_id 7
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

        ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);
        widget_bis->pick_id = 17;
        widget_3->pick_id = 27;

        ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print2, NULL);
        ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

        ei_linked_widget_t *head = ei_hashmap_widget_get_page(hashmap, 7);
        ei_hashmap_widget_remove(hashmap, widget_bis, NULL, NULL);
        assert(hashmap->length == 2 && "Test 1: The length of the hashmap should be 2.\n");
        assert(head->next->widget->pick_id == widget->pick_id && "Test 1: After removal of the second, the head's next should be the third.\n");
        puts("\tTest 1 OK");
        ei_hashmap_widget_destroy(hashmap);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 2: remove a callback inserted in the middle of a widget callback stack when multiple items in list");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL); // pick_id 10
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

        widget_bis->pick_id = 20;
        widget_3->pick_id = 30;

        ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);

        ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print3, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print2, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print, NULL);
        ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

        ei_linked_widget_t *head = ei_hashmap_widget_get_page(hashmap, 0);
        ei_hashmap_widget_remove(hashmap, widget_bis, print2, NULL);
        assert(hashmap->length == 3 && "Test 2: The length of the hashmap should be 3.\n");
        assert(head->next->widget->pick_id == widget_bis->pick_id && "Test 2: After removal of callback, the widget should be unaltered.\n");
        assert(head->next->linked_callbacks->callback == print && "Test 2: The first callback 'print' should have been left unaltered.\n");
        assert(head->next->linked_callbacks->next->callback == print3 && "Test 2: After removal of the second callback, the widget's first callback's next should be the third callback.\n");
        puts("\tTest 2 OK");
        ei_hashmap_widget_destroy(hashmap);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 3: remove a the first widget of a page of the hashmap");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL); // pick_id 13
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

        widget_bis->pick_id = 23;
        widget_3->pick_id = 33;

        ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);

        ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print2, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print, NULL);
        ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

        ei_hashmap_widget_remove(hashmap, widget_3, NULL, NULL);
        ei_linked_widget_t *head = ei_hashmap_widget_get_page(hashmap, 3);
        assert(hashmap->length == 2 && "Test 3: The length of the hashmap should be 2.\n");
        assert(head->widget->pick_id == widget_bis->pick_id && "Test 3: After removal of head, the new head should be the previous second.\n");
        assert(head->linked_callbacks->callback == print && "Test 3: The first callback 'print' of the previous second should have been left unaltered.\n");
        assert(head->linked_callbacks->next->callback == print2 && "Test 3: The second callback 'print2' of the previous second should have been left unaltered.\n");
        assert(head->next->widget->pick_id == widget->pick_id && "Test 3: After removal of head, the new second should be the previous third.\n");
        puts("\tTest 3 OK");
        ei_hashmap_widget_destroy(hashmap);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    puts("ei_hashmap_widget_remove PASSED");
}

void test_ei_hashmap_widget_find()
{
    puts("\nTesting ei_hashmap_widget_find ...");

    {
        printf("Test 1: find a widget when multiple items in list");
        // widget's pick_id is 16 here, do mod 10 to get its index in the hashmap
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL); // pick_id 16
        assert(widget->pick_id == 16 && "widget pick_id was not 16.\n");
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

        ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);

        ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print2, NULL);
        ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

        ei_linked_widget_t *found = ei_hashmap_widget_find(hashmap, widget);
        assert(hashmap->length == 3 && "Test 1: The length of the hashmap should be 3.\n");
        assert(widget->pick_id == found->widget->pick_id && "Test 1: Should have found the widget 'widget'.\n");
        puts("\tTest 1 OK");
        ei_hashmap_widget_destroy(hashmap);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 2: remove an item and try to find it afterward");
        // widget's pick_id is 19 here, do mod 10 to get its index in the hashmap
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        assert(widget->pick_id == 19 && "the widget pick_id should be 19.\n");
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

        ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);

        ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print3, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print2, NULL);
        ei_hashmap_widget_insert(hashmap, widget_bis, print, NULL);
        ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

        ei_hashmap_widget_remove(hashmap, widget_bis, NULL, NULL);
        ei_linked_widget_t *found = ei_hashmap_widget_find(hashmap, widget_bis);
        assert(hashmap->length == 2 && "Test 2: The length of the hashmap should be 2.\n");
        assert(found == NULL && "Test 2: The search of a removed widget should have returned NULL.\n");
        puts("\tTest 2 OK");
        ei_hashmap_widget_destroy(hashmap);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 3: find a widget wgt2 when there is an empty list between two widgets (index1: wgt1, index2: NULL, index3: wgt2)");
        // pick id is 22 here
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        assert(widget->pick_id == 22 && "the widget pick_id should be 22.\n");
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);

        ei_widget_hashmap_t *hashmap = ei_hashmap_widget_create(10);

        ei_hashmap_widget_insert(hashmap, widget, NULL, NULL);
        ei_hashmap_widget_insert(hashmap, widget_3, print2, NULL);

        ei_linked_widget_t *found = ei_hashmap_widget_find(hashmap, widget_3);
        ei_linked_widget_t *found_first = ei_hashmap_widget_find(hashmap, widget);
        assert(hashmap->length == 2 && "Test 3: The length of the hashmap should be 2.\n");
        assert(found->widget->pick_id == widget_3->pick_id && "Test 3: The search should have returned the widget 'widget_3'.\n");
        assert(found_first->widget->pick_id == widget->pick_id && "Test 3: The second search should have returned the widget 'widget'.\n");

        puts("\tTest 3 OK");
        ei_hashmap_widget_destroy(hashmap);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    puts("ei_hashmap_widget_find PASSED");
}
