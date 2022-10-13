#ifndef EI_LINKED_LIST_H
#define EI_LINKED_LIST_H

#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"

#include <stddef.h> // For NULL definition

/**
 * @brief   Free the memory used by a linked list.
 * Every element is expected to have a 'next' attribute.
 *
 * @param   first_link   The first link of the linked list to be freed.
 */
#define LINKED_LIST_DESTROY(LIST_TYPE, named_type) \
    void ei_##named_type##_destroy (LIST_TYPE *first_link) \
    { \
        for (LIST_TYPE *it = first_link; it != NULL;) \
        { \
            LIST_TYPE *tmp = (LIST_TYPE *) it->next; \
            free(it); \
            it = tmp; \
            \
        } \
    }
#endif