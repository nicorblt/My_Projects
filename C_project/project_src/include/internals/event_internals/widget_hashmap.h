#ifndef EVENT_INTERNAL_WIDGET_HASHMAP_H
#define EVENT_INTERNAL_WIDGET_HASHMAP_H

// TODO peut etre changer tous les widgets en pick_id dans ce fichier et linked_widget,c ar on n'utilise que pick_id du widget.

#include "internals/event_internals/linked_widget.h"

struct ei_iterator_t;

typedef struct ei_widget_hashmap_t
{
  ei_linked_widget_t **linked_widget_array;

  uint32_t length;
  uint32_t capacity;
} ei_widget_hashmap_t;


// create an empty hash map
ei_widget_hashmap_t *ei_hashmap_widget_create(uint32_t len);

// Callbacks order is Last in first out
// If widget "wgt" not already in the hashmap then it is inserted
// If callback is not NULL, then it is inserted, no matter if it is already registered.
void ei_hashmap_widget_insert(ei_widget_hashmap_t *hashmap, ei_widget_t* wgt, ei_callback_t callback, void *user_params);

// remove a widget and all its callback if callback is NULL
// otherwise, if callback non null, remove only this callback from widget.
void ei_hashmap_widget_remove(ei_widget_hashmap_t *hashmap, ei_widget_t *wgt, ei_callback_t callback, void *user_params);

ei_linked_widget_t *ei_hashmap_widget_get_page(ei_widget_hashmap_t *hashmap, uint32_t index);

// Destroy and free the entire hashmap
void ei_hashmap_widget_destroy(ei_widget_hashmap_t *hashmap);

// Find a specific widget in the hashmap
ei_linked_widget_t *ei_hashmap_widget_find(ei_widget_hashmap_t *hashmap, ei_widget_t *wgt);

/* ***** ITERATORS ***** */
void* ei_it_hashmap_next(struct ei_iterator_t *it);
void ei_it_hashmap_destroy(struct ei_iterator_t *it);

// iterator over a widget hashmap. If the hashmap is modified, the iterator behavior is undetermined.
struct ei_iterator_t ei_it_hashmap_begin(ei_widget_hashmap_t *hashmap);
ei_linked_widget_t *ei_it_hashmap_get(struct ei_iterator_t *it);

#endif