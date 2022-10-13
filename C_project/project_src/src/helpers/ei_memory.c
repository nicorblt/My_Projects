#include "helpers/ei_memory.h"

#include <stdlib.h>
#include <stdio.h>

#include "ei_types.h"

LINKED_LIST_DESTROY(ei_linked_point_t, linked_point)
LINKED_LIST_DESTROY(ei_linked_rect_t, linked_rect)


void *verified_calloc(size_t nmemb, size_t size)
{
    void *allocated = calloc(nmemb, size);
    if (allocated == NULL)
    {
        perror("Failed to allocate memory with calloc.");
        exit(1);
    }
    return allocated;
}

void *verified_malloc(size_t size)
{
    void *allocated = malloc(size);
    if (allocated == NULL)
    {
        perror("Failed to allocate memory with malloc.");
        exit(1);
    }
    return allocated;
}

void *verified_realloc(void *ptr, size_t size)
{
    void *allocated = realloc(ptr, size);
    if (allocated == NULL)
    {
        perror("Failed to allocate memory with realloc.");
        exit(1);
    }
    return allocated;
}