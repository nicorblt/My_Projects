#ifndef EI_MEMORY_H
#define EI_MEMORY_H

#include <stddef.h>
#include "iterators/ei_linked_list.h"

/**
 * @brief Allocate blocks of memory with a *calloc* and verify that it has been successfully allocated (otherwise abort the process).
 *
 * @param nmemb Number of elements to allocate (first parameter of calloc).
 * @param size The size of the block to allocate (second parameter of calloc).
 *
 * @return A pointer to the allocated memory.
 */
void *verified_calloc(size_t nmemb, size_t size);

/**
 * @brief Allocate a block of memory with a *malloc* and verify that it has been successfully allocated (otherwise abort the process).
 *
 * @param size The size of the block to allocate.
 *
 * @return A pointer to the allocated memory.
 */
void *verified_malloc(size_t size);

/**
 * @brief Extend or shrink a block of memory using a *realloc* and verify that it has been successfully allocated (otherwise abort the process).
 *
 * @param ptr Point to the memory block to resize to "size" bytes.
 * @param size The size of the block to allocate.
 *
 * @return A pointer to the allocated memory.
 */
void *verified_realloc(void *ptr, size_t size);

#endif