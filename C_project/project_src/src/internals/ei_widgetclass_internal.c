#include "internals/ei_widgetclass_internal.h"
#include "ei_widgetclass.h"
#include "iterators/ei_iterator.h"

/**
 * @brief   Allocates a new \ref ei_widgetclass_t.
 */
static ei_widgetclass_t *entry_widgetclass = NULL;

LINKED_LIST_DESTROY(ei_widgetclass_t, widgetclass)

ei_widgetclass_t * ei_widgetclass_get(void)
{
    return entry_widgetclass;
}

void ei_widgetclass_add(ei_widgetclass_t *to_insert)
{
    ei_widgetclass_t * tmp = entry_widgetclass;
    entry_widgetclass = to_insert;
    entry_widgetclass->next = tmp;
}

LIST_ITERATOR(ei_widgetclass_t, widgetclass)