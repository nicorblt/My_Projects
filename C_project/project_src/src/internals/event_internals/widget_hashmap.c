#include <assert.h>

#include "internals/event_internals/widget_hashmap.h"
#include "iterators/ei_iterator.h"
#include "helpers/ei_memory.h"
#include "helpers/ei_math.h"

#define HASHMAP_MIN_CAPACITY 10

// ********** STATIC ************

// hash method
static uint32_t hash_unsigned(uint32_t n)
{
  return n;
}

static uint32_t hashmap_widget_key_to_index(const ei_widget_hashmap_t *hashmap, uint32_t key)
{
  return hash_unsigned(key) % hashmap->capacity;
}

static ei_linked_widget_t **_ei_hashmap_widget_refresh(ei_widget_hashmap_t *hashmap, uint32_t new_cap)
{
  ei_linked_widget_t **tmp = verified_calloc(new_cap, sizeof(ei_linked_widget_t *));

  uint32_t linked_wgt_added = 0;
  for (size_t i = 0; linked_wgt_added < hashmap->length; i++)
  {
    ei_linked_widget_t *linked_widget = hashmap->linked_widget_array[i];
    while (linked_widget != NULL)
    {
      uint32_t key = linked_widget->widget->pick_id;
      uint32_t index = hash_unsigned(key) % new_cap;

      ei_linked_widget_t *old_next = linked_widget->next;
      linked_widget->next = tmp[index];
      tmp[index] = linked_widget;
      linked_widget = old_next;
      linked_wgt_added++;
    }
  }
  return tmp;
}

static void _ei_hashmap_widget_resize(ei_widget_hashmap_t *hashmap, uint32_t new_cap) 
{ 
  new_cap = MAX(new_cap, HASHMAP_MIN_CAPACITY); 
  
  if (new_cap == hashmap->capacity) 
    return; 
  
  ei_linked_widget_t **tmp = _ei_hashmap_widget_refresh(hashmap, new_cap); 
  
  hashmap->capacity = new_cap; 
  free(hashmap->linked_widget_array);
  hashmap->linked_widget_array = tmp; 
}

// ********* NON STATIC *************

// create an empty hash map
ei_widget_hashmap_t *ei_hashmap_widget_create(uint32_t len)
{
  ei_widget_hashmap_t * new = verified_calloc(1, sizeof(ei_widget_hashmap_t));

  new->length = 0;
  new->capacity = MAX(len, HASHMAP_MIN_CAPACITY);
  new->linked_widget_array = verified_calloc(new->capacity, sizeof(ei_linked_widget_t *));

  return new;
}


// Callbacks order is Last in first out
// If widget "wgt" not already in the hashmap then it is inserted
// If callback is not NULL, then it is inserted, no matter if it is already registered.
void ei_hashmap_widget_insert(ei_widget_hashmap_t *hashmap, ei_widget_t* wgt, ei_callback_t callback, void *user_params)
{
  uint32_t index = hashmap_widget_key_to_index(hashmap, wgt->pick_id);
  ei_linked_widget_t *new = ei_linked_widget_create(wgt);
  ei_linked_widget_t *inserted = ei_linked_widget_append(&hashmap->linked_widget_array[index], new);
  if (inserted != new) // a linked widget containing the same pick_id was found in the list, new wasn't inserted
    ei_linked_widget_destroy(new);
  else // insertion occurred
  {
    hashmap->length ++;
    if ((double) hashmap->length / hashmap->capacity >= 0.75)
      _ei_hashmap_widget_resize(hashmap, 2*hashmap->capacity);
  }

  if (callback != NULL)
    ei_linked_widget_register_callback(inserted, callback, user_params);
}

// remove a widget and all its callback if callback is NULL
// otherwise, if callback non null, remove only this callback from widget.
void ei_hashmap_widget_remove(ei_widget_hashmap_t *hashmap, ei_widget_t *wgt, ei_callback_t callback, void *user_params)
{
  if (wgt == NULL)
    return;

  uint32_t index = hashmap_widget_key_to_index(hashmap, wgt->pick_id);
  ei_linked_widget_t **entry_point = &hashmap->linked_widget_array[index];
  if (callback == NULL)
  {
    ei_bool_t successful_deletion = ei_linked_widget_remove(entry_point, wgt);
    hashmap->linked_widget_array[index] = *entry_point;
    if (successful_deletion) {
        hashmap->length--;
        if ((double) hashmap->length / hashmap->capacity <= 0.15)
            _ei_hashmap_widget_resize(hashmap, (uint32_t) (hashmap->capacity / 2));
    }
  }
  else 
  {
    ei_linked_widget_t *found = ei_linked_widget_find(*entry_point, wgt);
    if (found != NULL)
      ei_linked_widget_unregister_callback(found, callback, user_params);
  }
}

// Find a specific widget in the hashmap
// return NULL if not found, otherwise return the widget
ei_linked_widget_t *ei_hashmap_widget_find(ei_widget_hashmap_t *hashmap, ei_widget_t *wgt)
{
  ei_iterator_t linked_widget_it = ei_it_begin(hashmap, hashmap);
  for (; ei_it_valid(&linked_widget_it); ei_it_next(&linked_widget_it))
  {
    ei_linked_widget_t *linked = ei_it_get_t(hashmap, &linked_widget_it);
    if (linked->widget->pick_id == wgt->pick_id)
    {
      ei_it_destroy(&linked_widget_it);
      return linked;
    }
  }
  ei_it_destroy(&linked_widget_it);
  return NULL;
}

ei_linked_widget_t *ei_hashmap_widget_get_page(ei_widget_hashmap_t *hashmap, uint32_t index)
{
  assert(index < hashmap->capacity && "The index is out of range.\n");
  return hashmap->linked_widget_array[index];
}

// Destroy and free the entire hashmap
void ei_hashmap_widget_destroy(ei_widget_hashmap_t *hashmap)
{
  for (uint32_t i =0; i < hashmap->capacity; i++)
  {
    ei_linked_widget_destroy(hashmap->linked_widget_array[i]);
  }
  free(hashmap->linked_widget_array);
  free(hashmap);
}



/* *** iterator *** */

typedef struct hashmap_it_user_data_t
{
    uint32_t current_index;
    ei_widget_hashmap_t *hashmap;
} hashmap_it_user_data_t;



void* ei_it_hashmap_next(ei_iterator_t *it)
{
    void *result = NULL;
    if (it->data == it->off)
        return NULL;
    
    uint32_t current_index = ((hashmap_it_user_data_t *)it->user_data)->current_index;
    ei_widget_hashmap_t * hashmap = ((hashmap_it_user_data_t *)it->user_data)->hashmap;
    uint32_t capacity = hashmap->capacity;

    result = it->data;
    ei_linked_widget_t *linked = (ei_linked_widget_t *)it->data;
    if (linked->next != NULL)
        it->data = linked->next;
    else if (current_index + 1 < capacity)
    {
        it->data = linked->next;
        while (current_index + 1 < capacity && it->data == NULL)
        {
          current_index ++;
          it->data = hashmap->linked_widget_array[current_index];
        }
        ((hashmap_it_user_data_t *)it->user_data)->current_index = current_index;
        if (it->data == NULL)
        {
          it->data = it->off;
        }
    }
    else
    {
        it->data = it->off;
    }
    return result;
}


void ei_it_hashmap_destroy(ei_iterator_t *it)
{
    if (it->user_data != NULL)
        free((hashmap_it_user_data_t *)it->user_data);
}

// iterator over a widget hashmap. If the hashmap is modified, the iterator behavior is undetermined.
ei_iterator_t ei_it_hashmap_begin(ei_widget_hashmap_t *hashmap)
{
    if (hashmap == NULL || hashmap->length == 0)
    {
        return (ei_iterator_t) {NULL, NULL, NULL, ei_it_hashmap_next, ei_it_noop};
    }

    ei_iterator_t new;
    new.data = hashmap->linked_widget_array[0];
    new.off = NULL;
    uint32_t current_index = 0;
    while (new.data == NULL)
    {
      current_index ++;
      new.data = hashmap->linked_widget_array[current_index];
    }
    new.user_data = verified_calloc(1, sizeof(hashmap_it_user_data_t));
    ((hashmap_it_user_data_t *) new.user_data)->current_index = current_index; // current index
    ((hashmap_it_user_data_t *) new.user_data)->hashmap = hashmap; // current index

    new.next = ei_it_hashmap_next;
    new.destroyfunc = ei_it_hashmap_destroy;
    return new;
}

ei_linked_widget_t *ei_it_hashmap_get(ei_iterator_t *it)
{
    return (ei_linked_widget_t *) it->data;
}