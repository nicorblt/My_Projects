#ifndef EVENT_INTERNAL_LINKED_CALLBACK_H
#define EVENT_INTERNAL_LINKED_CALLBACK_H

#include "ei_widget.h"

struct ei_iterator_t;

// linked list callbacks

struct _ei_linked_callback_list_t;

typedef struct ei_linked_callback_t
{
  void *user_params;
  ei_callback_t callback;
  struct ei_linked_callback_t *next;
  struct ei_linked_callback_t **tail;
} ei_linked_callback_t;


// create empty linked list callback
ei_linked_callback_t *ei_linked_callback_create(ei_callback_t callback, void *user_params);

// Insert 'to_append" as the next element of the tail, then change the tail to to_append;
// Return the 'linked_list' if success, otherwise return NULL
ei_linked_callback_t *ei_linked_callback_append(ei_linked_callback_t **linked_callback, ei_linked_callback_t *to_append);

// delete a specific (callback + user_data) from the linked list. Only the first match found is removed and freed
void ei_linked_callback_remove(ei_linked_callback_t **head, ei_callback_t callback, void* user_params);

// free the entire linked list
void ei_linked_callback_destroy(ei_linked_callback_t *first_link);

/* ***** ITERATORS ***** */

void *ei_it_linked_callback_next(struct ei_iterator_t *it);
ei_linked_callback_t *ei_it_linked_callback_get(struct ei_iterator_t *it);
struct ei_iterator_t ei_it_linked_callback_begin(ei_linked_callback_t *iterable);

#endif