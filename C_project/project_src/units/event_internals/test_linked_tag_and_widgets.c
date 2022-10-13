#include <assert.h>
#include "internals/event_internals/linked_tag_and_widgets.h"
#include "ei_event.h"
#include <stdio.h>
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "internals/ei_widgetclass_internal.h"
#include "helpers/ei_memory.h"
#include "internals/event_internals/event_internal.h"

static ei_widget_t *root_widget = NULL;

void test_ei_linked_tag_and_widgets_append();
void test_ei_linked_tag_and_widgets_find();
void test_ei_linked_tag_and_widgets_register();
void test_ei_linked_tag_and_widgets_register_all_callback();
void test_ei_linked_tag_and_widgets_unregister_widget();
void test_ei_linked_tag_and_widgets_unregister_callback();
void test_ei_linked_tag_and_widgets_unregister_all_callback();
void test_ei_linked_tag_and_widgets_get_widget_callbacks();

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
    ei_button_register_class();
    ei_toplevel_register_class();
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

    test_ei_linked_tag_and_widgets_append();
    test_ei_linked_tag_and_widgets_find();
    test_ei_linked_tag_and_widgets_register();
    test_ei_linked_tag_and_widgets_register_all_callback();
    test_ei_linked_tag_and_widgets_unregister_widget();
    test_ei_linked_tag_and_widgets_unregister_callback();
    test_ei_linked_tag_and_widgets_unregister_all_callback();
    test_ei_linked_tag_and_widgets_get_widget_callbacks();
    ei_widget_destroy(root_widget);
    // destroy_event_array();
    ei_widgetclass_destroy(ei_widgetclass_get());
    return 0;
}

void test_ei_linked_tag_and_widgets_append()
{
    puts("\nTesting ei_linked_tag_and_widgets_append ...");

    {
        printf("Test 1: With 2 differents tag");
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *second = head;
        ei_linked_tag_and_widgets_t *first = ei_linked_tag_and_widgets_create("button");

        ei_linked_tag_and_widgets_t *inserted = ei_linked_tag_and_widgets_append(&head, first);

        assert(head->next == second && "Test 1: The added element's next value should be the previous head address.\n");
        assert(head == first && "Test 1: The added element should be new head.\n");
        assert(inserted == first && "Test 1: The returned link should be the one inserted !\n");
        puts("\tTest 1 OK.");
        ei_linked_tag_and_widgets_destroy(head);
    }

    printf("Test 2: with 2 equal tags and another one.");

    ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
    ei_linked_tag_and_widgets_t *third = head;
    ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
    ei_linked_tag_and_widgets_t *first = ei_linked_tag_and_widgets_create("button");
    ei_linked_tag_and_widgets_t *inserted = ei_linked_tag_and_widgets_append(&head, second);
    ei_linked_tag_and_widgets_append(&head, first);

    assert(head->next == third && "Test 2: No insertion should have occured.\n");
    assert(head->next->next == NULL && "Test 2: No second insertion should have occured.\n");
    assert(head == first && "Test 2: The head should be link 'first'.\n");
    assert(inserted == third && "Test 2: The return value of ei_linked_tag_and_widgets_append should be the already inserted widget.\n");
    puts("\tTest 2 OK.");
    ei_linked_tag_and_widgets_destroy(second);
    ei_linked_tag_and_widgets_destroy(head);

    puts("ei_linked_tag_and_widgets_append PASSED.");
}

void test_ei_linked_tag_and_widgets_register()
{
    puts("\nTesting ei_linked_tag_and_widgets_register ...");

    {
        printf("Test 1: Registration of a first widget and callback in the middle of the list.");
        ei_widget_t *widget = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print, NULL);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->callback == print && "Test 1: The callback 'print' was not correctly inserted.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 1: The widget was not inserted in the second link.\n");
        assert(first->associated_widgets->length == 0 && "Test 1: The link 'first' should have no widget.\n");
        assert(head->associated_widgets->length == 0 && "Test 1: The head link should have no widget.\n");
        puts("\tTest 1 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
    }

    {
        printf("Test 2: Registration of two widgets in the middle of the list, and one at the end of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, NULL, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, NULL, NULL);

        ei_linked_tag_and_widgets_register(first, widget_3, NULL, NULL);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 2: The widget 'widget' was not inserted in the second link.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis) != NULL && "Test 2: The widget 'widget_bis' was not inserted in the second link.\n");
        assert(ei_hashmap_widget_find(first->associated_widgets, widget_3) != NULL && "Test 2: The widget 'widget_3' was not inserted in the link 'first'.\n");
        assert(head->associated_widgets->length == 0 && "Test 2: The head link should have no widget.\n");

        puts("\tTest 2 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 3: Registration of two widgets in the middle of the list, one with multiple callbacks, the other with none, and one widget at the end of the list.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print2, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, NULL, NULL);

        ei_linked_tag_and_widgets_register(first, widget_3, print3, NULL);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 3: The widget 'widget' was not inserted in the second link.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->callback == print && "Test 3: The callback 'print' was not correctly inserted at the first place.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->next->callback == print2 && "Test 3: The callback 'print2' was not correctly inserted next to 'print'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis) != NULL && "Test 3: The widget 'widget_bis' was not inserted in the second link.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis)->linked_callbacks == NULL && "Test 3: The widget 'widget_bis' should have no linked callback..\n");
        assert(ei_hashmap_widget_find(first->associated_widgets, widget_3) != NULL && "Test 3: The widget 'widget_3' was not inserted in the link 'first'.\n");
        assert(ei_hashmap_widget_find(first->associated_widgets, widget_3)->linked_callbacks->callback == print3 && "Test 3: The callback 'print3' was not correctly inserted for widget 'widget_3'.\n");
        assert(head->associated_widgets->length == 0 && "Test 3: The head link should have no widget.\n");

        puts("\tTest 3 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }
    puts("ei_linked_tag_and_widgets_register PASSED");
}

void test_ei_linked_tag_and_widgets_register_all_callback()
{
    puts("\nTesting ei_linked_tag_and_widgets_register_all_callback ...");

    {
        printf("Test 1: Registration of a callback to all widgets, with all widgets having no callback.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, NULL, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, NULL, NULL);

        ei_linked_tag_and_widgets_register_all_callback(second, print, NULL);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 1: The widget 'widget' was not inserted in the second link.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->callback == print && "Test 1: The widget 'widget' should have the 'print' callback linked.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis) != NULL && "Test 1: The widget 'widget_bis' was not inserted in the second link.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis)->linked_callbacks->callback == print && "Test 1: The widget 'widget_bis' should have the 'print' callback linked.\n");
        assert(head->associated_widgets->length == 0 && "Test 1: The head link should have no widget.\n");
        assert(first->associated_widgets->length == 0 && "Test 1: The link 'first' should have no widget.\n");

        puts("\tTest 1 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 2: Registration of a callback to all widgets, with a widget having no callback, one having multiple, and a third having only one.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print3, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print2, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, NULL, NULL);
        ei_linked_tag_and_widgets_register(second, widget_3, print2, NULL);

        ei_linked_tag_and_widgets_register_all_callback(second, print, NULL);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->callback == print && "Test 3: The callback 'print' was not correctly inserted at the first place of 'widget'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->next->callback == print2 && "Test 3: The callback 'print2' was not correctly inserted at the second place of 'widget'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->next->next->callback == print3 && "Test 3: The callback 'print3' was not correctly inserted at the third place 'widget'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis)->linked_callbacks->callback == print && "Test 3: The widget 'widget_bis' should now have the 'print' callback registered.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis)->linked_callbacks->next == NULL && "Test 3: The widget 'widget_bis' should only have one callback registered.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_3)->linked_callbacks->callback == print && "Test 3: The callback 'print' was not correctly inserted at the first place of 'widget_3'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_3)->linked_callbacks->next->callback == print2 && "Test 3: The callback 'print2' was not correctly inserted at the second place 'widget_3'.\n");
        assert(head->associated_widgets->length == 0 && "Test 1: The head link should have no widget.\n");
        assert(first->associated_widgets->length == 0 && "Test 1: The link 'first' should have no widget.\n");

        puts("\tTest 3 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }
    puts("ei_linked_tag_and_widgets_register_all_callback PASSED");
}

void test_ei_linked_tag_and_widgets_unregister_widget()
{
    puts("\nTesting ei_linked_tag_and_widgets_unregister_widget ...");

    {
        printf("Test 1: Registration and removal of a first widget and callback in the middle of the list.");
        ei_widget_t *widget = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_unregister_widget(second, widget);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget) == NULL && "Test 1: A search on the widget should have returned NULL.\n");
        assert(first->associated_widgets->length == 0 && "Test 1: The link 'first' should have no widget.\n");
        assert(second->associated_widgets->length == 0 && "Test 1: The link 'second' should have no widget.\n");
        assert(head->associated_widgets->length == 0 && "Test 1: The head link should have no widget.\n");
        puts("\tTest 1 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
    }

    {
        printf("Test 2: Registration of two widgets in the middle of the list, and one at the end of the list. Removal of first among the two first");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, NULL, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, NULL, NULL);

        ei_linked_tag_and_widgets_register(first, widget_3, NULL, NULL);
        ei_linked_tag_and_widgets_unregister_widget(second, widget_bis);

        assert(second->associated_widgets->length == 1 && "Test 2: The second link should have a length of 1 and not 2 after unregistration of 'widget_bis'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 2: The widget 'widget' was no longer found after deletion of 'widget_bis'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis) == NULL && "Test 2: The widget 'widget_bis' should not be found after unregistration\n");
        assert(ei_hashmap_widget_find(first->associated_widgets, widget_3) != NULL && "Test 2: The widget 'widget_3' was not inserted in the link 'first'.\n");
        assert(head->associated_widgets->length == 0 && "Test 2: The head link should have no widget.\n");

        puts("\tTest 2 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }
    puts("ei_linked_tag_and_widgets_unregister_widget PASSED");
}

void test_ei_linked_tag_and_widgets_unregister_callback()
{
    puts("\nTesting ei_linked_tag_and_widgets_unregister_callback ...");

    {
        printf("Test 1: Registration of a first widget and callback in the middle of the list. Removal of the callback");
        ei_widget_t *widget = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_unregister_callback(second, widget, print, NULL);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks == NULL && "Test 1: The callback 'print' was not correctly unregistered.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 1: The widget was no longer in the second link.\n");
        assert(first->associated_widgets->length == 0 && "Test 1: The link 'first' should have no widget.\n");
        assert(head->associated_widgets->length == 0 && "Test 1: The head link should have no widget.\n");
        puts("\tTest 1 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
    }

    {
        printf("Test 2: Registration of two widgets in the middle of the list, one with multiple callbacks, and one widget at the end of the list with two callbacks. Removal of two callbacks among the three.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print3, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print2, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, print, NULL);
        ei_linked_tag_and_widgets_register(first, widget_3, print2, NULL);
        ei_linked_tag_and_widgets_register(first, widget_3, print, NULL);

        ei_linked_tag_and_widgets_unregister_callback(second, widget, print2, NULL);
        ei_linked_tag_and_widgets_unregister_callback(second, widget, print, NULL);
        ei_linked_tag_and_widgets_unregister_callback(first, widget_3, print2, NULL);

        assert(second->associated_widgets->length == 2 && "Test 2: The link 'second' should still have 2 widgets.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 2: In link 'second', the widget 'widget' should still exist.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->next == NULL && "Test 2: There should only be 1 callback in link 'second', widget 'widget'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->callback == print3 && "Test 2: The callback 'print3' should be present in the widget 'widget' of link 'second'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis)->linked_callbacks->callback == print && "Test 2: The callback 'print' of 'widget_bis' in the link 'second' should have been preserved.\n");
        assert(first->associated_widgets->length == 1 && "Test 2: The link 'first' should still have 1 widgets.\n");
        assert(ei_hashmap_widget_find(first->associated_widgets, widget_3) != NULL && "Test 2: The widget 'widget_3' should still exist in link 'first'.\n");
        assert(ei_hashmap_widget_find(first->associated_widgets, widget_3)->linked_callbacks->callback == print && "Test 2: The callback 'print' was not preserved in 'widget_3' of link 'first'.\n");
        assert(ei_hashmap_widget_find(first->associated_widgets, widget_3)->linked_callbacks->next == NULL && "Test 2: 'widget_3' of link 'first' should only have 1 callback left.\n");
        assert(head->associated_widgets->length == 0 && "Test 2: The head link should have no widget.\n");

        puts("\tTest 2 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    puts("ei_linked_tag_and_widgets_unregister_callback PASSED");
}

void test_ei_linked_tag_and_widgets_unregister_all_callback()
{
    puts("\nTesting ei_linked_tag_and_widgets_unregister_all_callback ...");

    {
        printf("Test 1: Registration of a first widget and callback in the middle of the list. Removal of the callback to all widgets");
        ei_widget_t *widget = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_unregister_all_callback(second, print, NULL);

        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks == NULL && "Test 1: The callback 'print' was not correctly unregistered.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 1: The widget was no longer in the second link.\n");
        assert(first->associated_widgets->length == 0 && "Test 1: The link 'first' should have no widget.\n");
        assert(head->associated_widgets->length == 0 && "Test 1: The head link should have no widget.\n");
        puts("\tTest 1 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
    }

    {
        printf("Test 2: Registration of a link having multiple shared callback. Removal of all shared callbacks.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print3, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print2, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, print, NULL);
        ei_linked_tag_and_widgets_register(second, widget_3, print2, NULL);
        ei_linked_tag_and_widgets_register(second, widget_3, print, NULL);

        ei_linked_tag_and_widgets_unregister_all_callback(second, print, NULL);
        ei_linked_tag_and_widgets_unregister_all_callback(second, print2, NULL);

        assert(second->associated_widgets->length == 3 && "Test 2: The link 'second' should still have 3 widgets.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget) != NULL && "Test 2: In link 'second', the widget 'widget' should still exist.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->next == NULL && "Test 2: There should only be 1 callback in link 'second', widget 'widget'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget)->linked_callbacks->callback == print3 && "Test 2: The callback 'print3' should be present in the widget 'widget' of link 'second'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_bis)->linked_callbacks == NULL && "Test 2: 'widget_bis' of link 'second' should have no callbacks left.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_3) != NULL && "Test 2: The widget 'widget_3' should still exist in link 'second'.\n");
        assert(ei_hashmap_widget_find(second->associated_widgets, widget_3)->linked_callbacks == NULL && "Test 2: 'widget_3' of link 'second' should have no callback left.\n");
        assert(first->associated_widgets->length == 0 && "Test 2: The link 'first' should have no widgets.\n");
        assert(head->associated_widgets->length == 0 && "Test 2: The head link should have no widget.\n");

        puts("\tTest 2 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    puts("ei_linked_tag_and_widgets_unregister_all_callback PASSED");
}

void test_ei_linked_tag_and_widgets_find()
{
    puts("\nTesting ei_linked_tag_and_widgets_find ...");

    {
        printf("Test 1: Search tag in the middle of the list.");
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_t *found = ei_linked_tag_and_widgets_find(head, "button");

        assert(found == second && "Test 1: Should have found the second link.\n");
        puts("\tTest 1 OK");
        ei_linked_tag_and_widgets_destroy(head);
    }

    puts("Test 2 NOT FOUND");
    // {
    // printf("Test 2: Search tag after removal in the middle of the list.");
    // ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
    // ei_linked_tag_and_widgets_t *first = head;
    // ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("button");
    // ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("toplevel");
    // ei_linked_tag_and_widgets_append(&head, second);
    // ei_linked_tag_and_widgets_append(&head, third);
    //
    // ei_linked_tag_and_widgets_remove(&head, "button");
    // ei_linked_tag_and_widgets_t *found = ei_linked_tag_and_widgets_find(head, "button");
    //
    // assert(found == NULL && "Test 2: After removal of a tag, a search upon it should return 'NULL'.\n");
    // puts("\tTest 2 OK");
    // ei_linked_tag_and_widgets_destroy(head);
    // }

    {
        printf("Test 3: Search tag at the end of the list.");
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *previous_head = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_t *found = ei_linked_tag_and_widgets_find(head, "frame");

        assert(found == previous_head && "Test 3: Should have found the third link.\n");
        puts("\tTest 3 OK");
        ei_linked_tag_and_widgets_destroy(head);
    }

    {
        printf("Test 4: Search tag at the beginning of the list.");
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *previous_head = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_t *found = ei_linked_tag_and_widgets_find(head, "toplevel");

        assert(found == third && "Test 4: Should have found the first link.\n");
        puts("\tTest 4 OK");
        ei_linked_tag_and_widgets_destroy(head);
    }
    puts("ei_linked_tag_and_widgets_find PASSED");
}

void test_ei_linked_tag_and_widgets_get_widget_callbacks()
{
    puts("\nTesting ei_linked_tag_and_widgets_get_widget_callbacks ...");

    {
        printf("Test 1: Get callbacks when there is none.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *first = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, NULL, NULL);

        ei_linked_callback_t *found = ei_linked_tag_and_widgets_get_widget_callbacks(second, widget);

        assert(found == NULL && "Test 1: Should have found no callbacks.\n");
        puts("\tTest 1 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 2: Get callbacks when multiple are attached to the widget.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_t *previous_head = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print2, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, print3, NULL);

        ei_linked_callback_t *found = ei_linked_tag_and_widgets_get_widget_callbacks(second, widget);

        assert(found->callback == print && "Test 2: The found callbacks should start by 'print'.\n");
        assert(found->next->callback == print2 && "Test 2: The found callbacks should have 'print2' at second position.\n");
        assert(found->next->next == NULL && "Test 2: The found callbacks should have only two callbacks.\n");
        puts("\tTest 2 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }

    {
        printf("Test 3: Get callbacks of a non regsitered widget.");
        ei_widget_t *widget = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("frame", root_widget, NULL, NULL);
        ei_widget_t *widget_3 = ei_widget_create("toplevel", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *head = ei_linked_tag_and_widgets_create("button");
        ei_linked_tag_and_widgets_t *previous_head = head;
        ei_linked_tag_and_widgets_t *second = ei_linked_tag_and_widgets_create("frame");
        ei_linked_tag_and_widgets_t *third = ei_linked_tag_and_widgets_create("toplevel");
        ei_linked_tag_and_widgets_append(&head, second);
        ei_linked_tag_and_widgets_append(&head, third);

        ei_linked_tag_and_widgets_register(second, widget, print2, NULL);
        ei_linked_tag_and_widgets_register(second, widget, print, NULL);
        ei_linked_tag_and_widgets_register(second, widget_bis, print3, NULL);

        ei_linked_callback_t *found = ei_linked_tag_and_widgets_get_widget_callbacks(second, widget_3);

        assert(found == NULL && "Test 3: The search should have returned no callbacks.\n");
        puts("\tTest 3 OK");
        ei_linked_tag_and_widgets_destroy(head);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        ei_widget_destroy(widget_3);
    }
    puts("ei_linked_tag_and_widgets_get_widget_callbacks PASSED");
}
