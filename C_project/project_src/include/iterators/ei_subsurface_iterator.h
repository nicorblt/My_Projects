#ifndef EI_SUBSURFACE_ITERATOR_H
#define EI_SUBSURFACE_ITERATOR_H

#include <stddef.h>
#include "iterators/ei_iterator.h"

/**
 * @brief Create a new iterator over a surface.
 * 
 * @param surface A surface \ref ei_surface_t.
 * @param clipped_area A \ref ei_rect_t resulting from a call of intersect_rect() on the above surface.
 * @param ... Parameters of unknown types and quantity that may be useful for determining the first iterator over the surface.
 * @return \ref ei_iterator_t 
 */
ei_iterator_t ei_it_subsurface_begin(void* surface, ...);
void * ei_it_subsurface_next(ei_iterator_t * iterator);
void ei_it_subsurface_destroy(ei_iterator_t *it);
uint32_t* ei_it_subsurface_get(ei_iterator_t *it);

#endif