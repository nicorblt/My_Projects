#include <assert.h>
#include <stdio.h>

#include "internals/event_internals/linked_tag_and_widgets.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "internals/ei_widgetclass_internal.h"
#include "helpers/ei_string.h"
#include "helpers/ei_memory.h"
#include "internals/event_internals/event_internal.h"

static ei_widget_t *root_widget = NULL;

void test_create_event_array();
void test_get_event_array();
void test_get_event_array_length();
void test_ei_call_callbacks();
void test_ei_manage_event();

static int foo_value = 0;
static int goo_value = 0;
static int hoo_value = 0;

ei_bool_t foo(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    foo_value++;
    return EI_FALSE;
}

ei_bool_t goo(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    goo_value++;
    return EI_TRUE;
}

ei_bool_t hoo(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    hoo_value++;
    return EI_FALSE;
}

int main()
{
    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();
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

    test_create_event_array();
    test_get_event_array();
    test_get_event_array_length();
    test_ei_call_callbacks();
    // test_ei_manage_event();

    ei_widget_destroy(root_widget);
    ei_widgetclass_destroy(ei_widgetclass_get());
    return 0;
}

void test_create_event_array()
{
    puts("\nTesting create_event_array ...");

    {
        printf("Test 1: First call should initialize the linked tag and widgets");

        create_event_array();

        for (size_t i = 0; i < ei_ev_last - 1; i++)
        {
            ei_linked_tag_and_widgets_t *head = get_event_array(i + 1);
            assert(head != NULL && "Test 1: Every element of the event_array should have a linked_tag_and_widgets.\n");
            assert(STRCMP(head->tag, ==, "frame") && "Test 1: Frame should the tag of the first link.\n");
            assert(STRCMP(head->next->tag, ==, "button") && "Test 1: Button should the tag of the second link.\n");
            assert(STRCMP(head->next->next->tag, ==, "toplevel") && "Test 1: Toplevel should the tag of the third link.\n");
            assert(head->next->next->next == NULL && "Test 1: There should be only three linked tag.\n");
        }
        puts("\tTest 1 OK.");
        destroy_event_array();
    }

    puts("create_event_array PASSED.");
}

void test_get_event_array()
{
    puts("\nTesting get_event_array ...");
    create_event_array();
    {
        printf("Test 1: Test every valid index");

        for (size_t i = 0; i < get_event_array_length(); i++)
        {
            ei_linked_tag_and_widgets_t *head = get_event_array(i + 1);
            assert(head != NULL && "Test 1: Every element in bounds of the event_array valid indexes should be non NULL.\n");
        }

        puts("\tTest 1 OK.");
    }
    {
        printf("Test 2: Test just out of bounds indexes");

        ei_linked_tag_and_widgets_t *head = get_event_array(0);
        assert(head == NULL && "Test 2: Calling get_event_array with ei_ev_none (=0) should return NULL.\n");
        head = get_event_array(ei_ev_last);
        assert(head == NULL && "Test 2: Calling get_event_array with ei_ev_last should return NULL.\n");

        puts("\tTest 2 OK.");
    }
    destroy_event_array();
    puts("get_event_array PASSED.");
}

void test_get_event_array_length()
{
    puts("\n Testing get_event_array_length ...");
    printf("Test 1: Only test.");
    assert(get_event_array_length() == ei_ev_last - 1 && "The length of the event_array should be the number of element of the enum ei_eventtype_t minus ei_ev_last and ei_ev_none.\n");
    puts("\tTest 1 OK.");
    puts("get_event_array_length PASSED.");
}

void test_ei_call_callbacks()
{
    puts("\nTesting ei_call_callbacks ...");
    create_event_array();
    {
        printf("Test 1: With only one callback");
        ei_widget_t *widget = ei_widget_create("button", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *link = get_event_array(ei_ev_none + 1);
        link = ei_linked_tag_and_widgets_find(link, "button");
        ei_linked_tag_and_widgets_register(link, widget, foo, NULL);

        foo_value = 0;
        ei_event_t event;
        event.type = ei_ev_none + 1;
        ei_call_callbacks(&event, widget);

        assert(foo_value == 1 && "The 'foo' callback should have been called exactly once.\n");
        foo_value = 0;

        ei_linked_tag_and_widgets_unregister_widget(link, widget);
        ei_widget_destroy(widget);
        puts("\tTest 1 OK.");
    }
    {
        printf("Test 2: Test with multiple widgets in same list, each having multiple callbacks.");
        ei_widget_t *widget = ei_widget_create("button", root_widget, NULL, NULL);
        ei_widget_t *widget_bis = ei_widget_create("button", root_widget, NULL, NULL);
        ei_linked_tag_and_widgets_t *link = get_event_array(ei_ev_none + 1);
        link = ei_linked_tag_and_widgets_find(link, "button");
        ei_linked_tag_and_widgets_register(link, widget, hoo, NULL);
        ei_linked_tag_and_widgets_register(link, widget, goo, NULL);
        ei_linked_tag_and_widgets_register(link, widget, foo, NULL);
        ei_linked_tag_and_widgets_register(link, widget_bis, foo, NULL);

        foo_value = 0;
        goo_value = 0;
        hoo_value = 0;
        ei_event_t event;
        event.type = ei_ev_none + 1;
        ei_call_callbacks(&event, widget);

        assert(foo_value == 1 && "The 'foo' callback should have been called exactly once.\n");
        assert(goo_value == 1 && "The 'goo' callback should have been called exactly once.\n");
        assert(hoo_value == 0 && "Because 'goo' returns TRUE and callbacks are LIFO, the 'hoo' callback should not be called.\n");
        foo_value = 0;
        goo_value = 0;
        hoo_value = 0;

        ei_linked_tag_and_widgets_unregister_widget(link, widget);
        ei_linked_tag_and_widgets_unregister_widget(link, widget_bis);
        ei_widget_destroy(widget);
        ei_widget_destroy(widget_bis);
        puts("\tTest 2 OK.");
    }
    destroy_event_array();
    puts("ei_call_callbacks PASSED.");
}