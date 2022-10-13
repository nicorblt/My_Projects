#ifndef EI_WIDGETCLASS_INTERNAL_H
#define EI_WIDGETCLASS_INTERNAL_H

#include "iterators/ei_linked_list.h"

struct ei_widgetclass_t;
struct ei_iterator_t;

void ei_widgetclass_destroy(struct ei_widgetclass_t *first);
void ei_widgetclass_add(ei_widgetclass_t *to_insert);
ei_widgetclass_t* ei_widgetclass_get(void);


void *ei_it_widgetclass_next(struct ei_iterator_t *it);
ei_widgetclass_t *ei_it_widgetclass_get(struct ei_iterator_t *it);
struct ei_iterator_t ei_it_widgetclass_begin(ei_widgetclass_t *iterable);


#endif