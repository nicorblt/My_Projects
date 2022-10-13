#ifndef EI_ITERATOR_H
#define EI_ITERATOR_H

#include <stdarg.h>
#include <stddef.h>

#include "ei_types.h"

struct ei_iterator_t;

/**
 * @brief Return the next element of the iterator or NULL if the iterator is fully consumed.
 *
 * @param it The iterator from which to return the next element.
 */
typedef void *(*ei_iterator_nextfunc_t)(struct ei_iterator_t *it);

// /**
//  * @brief Return the an invalid iterator right after the last valid iterator over the iterable.
//  *
//  * @param iterable The iterable being iterated through.
//  */
// typedef const struct ei_iterator_t* (*ei_iterator_endfunc_t)(void * iterable, ...);

/**
 * @brief Release all memory owned by the iterator and finally free the iterator itself.
 * At the end of this function, the iterator is not valid anymore.
 * @param it A pointer on the iterator to be destroyed.
 */
typedef void (*ei_iterator_destroyfunc_t)(struct ei_iterator_t *it);

typedef struct ei_iterator_t
{
    void *data;
    void *off;

    void *user_data;

    ei_iterator_nextfunc_t next;
    ei_iterator_destroyfunc_t destroyfunc;

} ei_iterator_t;

static inline void *ei_it_next(ei_iterator_t *it)
{
    return it->next(it);
}

static inline void *ei_it_get(ei_iterator_t *it)
{
    return it->data;
}

static inline void ei_it_destroy(ei_iterator_t *it)
{
    if (it->destroyfunc != NULL)
        it->destroyfunc(it);
}
// TODO demander au prof si cette liberté est autorisée
static inline void ei_it_noop(SDL_UNUSED ei_iterator_t *it) {}

static inline ei_bool_t ei_it_valid(ei_iterator_t *it)
{
    return it->data != it->off;
}

/**
 * @brief Create a new iterator over an iterable.
 *
 * @param iterable An object you can iterate over.
 * @param ... Parameters of unknown types and quantity that may be useful for determining the first iterator over the iterable.
 * @return struct ei_iterator_t*
 */
#define ei_it_begin(named_type, ...) ei_it_##named_type##_begin(__VA_ARGS__)

/**
 * @brief Return the typed data the iterator is currently on. This call is specific to each iterator and returns a typed value.
 *
 */
#define ei_it_get_t(named_type, iterator_ptr) ei_it_##named_type##_get(iterator_ptr)

/**
 * @brief
 *
 * @param TYPE Is the type of the data contained by the iterable.
 * @param named_type Is the the name of the iterator we want to create.
 *
 */
#define ARRAY_ITERATOR(TYPE, named_type)                                                    \
    void *ei_it_##named_type##_next(ei_iterator_t *it)                                      \
    {                                                                                       \
        void *result = NULL;                                                                \
        if (it->data != it->off)                                                            \
        {                                                                                   \
            result = it->data;                                                              \
            it->data = (TYPE *)it->data + 1;                                                \
        }                                                                                   \
        return result;                                                                      \
    }                                                                                       \
    TYPE *ei_it_##named_type##_get(ei_iterator_t *it)                                       \
    {                                                                                       \
        return (TYPE *)it->data;                                                            \
    }                                                                                       \
    ei_iterator_t ei_it_##named_type##_begin(TYPE *iterable, TYPE *off)                     \
    {                                                                                       \
        return (ei_iterator_t){iterable, off, NULL, ei_it_##named_type##_next, ei_it_noop}; \
    }

/**
 * @brief
 *
 * @param TYPE Is the type of the data contained by the iterable.
 * @param named_type Is the the name of the iterator we want to create.
 *
 */
#define LIST_ITERATOR(TYPE, named_type)                                                      \
    void *ei_it_##named_type##_next(ei_iterator_t *it)                                       \
    {                                                                                        \
        void *result = NULL;                                                                 \
        if (it->data != it->off)                                                             \
        {                                                                                    \
            result = it->data;                                                               \
            it->data = ((TYPE *)it->data)->next;                                             \
        }                                                                                    \
        return result;                                                                       \
    }                                                                                        \
    TYPE *ei_it_##named_type##_get(ei_iterator_t *it)                                        \
    {                                                                                        \
        return (TYPE *)it->data;                                                             \
    }                                                                                        \
    ei_iterator_t ei_it_##named_type##_begin(TYPE *iterable)                                 \
    {                                                                                        \
        return (ei_iterator_t){iterable, NULL, NULL, ei_it_##named_type##_next, ei_it_noop}; \
    }

#endif