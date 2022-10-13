#include <assert.h>
#include "internals/event_internals/linked_callback.h"
#include "ei_event.h"
#include <stdio.h>

void test_ei_linked_callback_append();
void test_ei_linked_callback_remove();

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
    test_ei_linked_callback_append();
    test_ei_linked_callback_remove();
    return 0;
}

void test_ei_linked_callback_append()
{
    puts("\nTesting ei_linked_callback_append ...");
    ei_linked_callback_t *link = ei_linked_callback_create(print, NULL);
    ei_linked_callback_t *previous_head = link;
    ei_linked_callback_t *to_append = ei_linked_callback_create(print2, NULL);

    ei_linked_callback_append(&link, to_append);

    assert(link->next == previous_head && "The added element's next value should be the previous head address.\n");
    assert(link == to_append && "The added element should be new head.\n");
    puts("ei_linked_callback_append PASSED.");
    ei_linked_callback_destroy(link);
}

void test_ei_linked_callback_remove()
{
    puts("\nTesting ei_linked_callback_remove ...");

    {
        printf("Test 1: remove when multiple items in list");
        ei_linked_callback_t *head = ei_linked_callback_create(print, NULL);
        ei_linked_callback_t *third = head;
        ei_linked_callback_t *second = ei_linked_callback_create(print2, NULL);
        ei_linked_callback_t *first = ei_linked_callback_create(print, NULL);
        ei_linked_callback_append(&head, second);
        ei_linked_callback_append(&head, first);

        ei_linked_callback_remove(&head, print2, NULL);
        assert(head->next == third && "Test 1: The element with callback 'print2' was not correctly removed \n.");
        puts("\tTest 1 OK");
        ei_linked_callback_destroy(head);
    }

    {
        printf("Test 1: remove the head when multiple items in list");
        ei_linked_callback_t *head = ei_linked_callback_create(print2, NULL);
        ei_linked_callback_t *third = head;
        ei_linked_callback_t *second = ei_linked_callback_create(print2, NULL);
        ei_linked_callback_t *first = ei_linked_callback_create(print, NULL);
        ei_linked_callback_append(&head, second);
        ei_linked_callback_append(&head, first);

        ei_linked_callback_remove(&head, print, NULL);
        assert(head == second && "Test 2: Removal of the head, the new head should be the previous second element\n.");
        assert(head->next == third && "Test 2: Removal of the head, the new head's next element should be unaltered\n.");
        puts("\tTest 2 OK");
        ei_linked_callback_destroy(head);
    }
    puts("ei_linked_callback_remove PASSED.");
}
