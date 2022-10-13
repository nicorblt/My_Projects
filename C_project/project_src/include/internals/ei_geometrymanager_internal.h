#ifndef EI_GEOMETRYMANAGER_INTERNAL_H
#define EI_GEOMETRYMANAGER_INTERNAL_H

#include "iterators/ei_linked_list.h"

struct ei_geometrymanager_t;

void ei_geometrymanager_destroy(struct ei_geometrymanager_t *first);
void ei_geometrymanager_add(ei_geometrymanager_t *to_insert);
ei_geometrymanager_t* ei_geometrymanager_get(void);

#endif