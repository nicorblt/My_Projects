#include "internals/ei_geometrymanager_internal.h"
#include "ei_geometrymanager.h"


/**
 * @brief   Entry point of the linked list of every registered \ref ei_geometrymanager_t.
 */
static ei_geometrymanager_t * entry_geometrymanager = NULL;

LINKED_LIST_DESTROY(ei_geometrymanager_t, geometrymanager)

ei_geometrymanager_t * ei_geometrymanager_get(void)
{
    return entry_geometrymanager;
}

void ei_geometrymanager_add(ei_geometrymanager_t *to_insert)
{
    ei_geometrymanager_t * tmp = entry_geometrymanager;
    entry_geometrymanager = to_insert;
    entry_geometrymanager->next = tmp;
}