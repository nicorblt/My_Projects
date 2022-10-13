#include "iterators/ei_widget_tree_iterator.h"
#include "iterators/ei_iterator.h"
#include "ei_widget.h"



ei_iterator_t ei_it_widget_tree_begin(void* root_widget, ...)
{    
    ei_iterator_t it;

    if (root_widget == NULL)
    {
        return (ei_iterator_t) {NULL, NULL, NULL, ei_it_widget_tree_next, ei_it_noop};
    }

	it.data = root_widget;
	it.next = ei_it_widget_tree_next;
	it.destroyfunc = ei_it_noop;
	it.off = ((ei_widget_t *) root_widget)->parent; // retenir le root_widget comme point d'arret
    it.user_data = NULL;
    return it;
}


/**
 * @brief Returns left-most leave first, right-most leave last ( depth first search )
 * 
 * @param iterator 
 * @return void* 
 */
void * ei_it_widget_tree_next(ei_iterator_t *iterator)
{
    void * result = NULL;

    // end of iterator
    if (iterator->data != iterator->off)
    {
        ei_widget_t * wgt = (ei_widget_t *) iterator->data;
        result = iterator->data;
        if (wgt->children_head != NULL)
        {
            iterator->data = (void *) wgt->children_head;
        }
        else if (wgt->next_sibling != NULL)
        {
            iterator->data = (void *) wgt->next_sibling;
        }
        else
        {
            ei_widget_t *it = wgt;
            while (it->parent != iterator->off)
            {
                if (it->parent->next_sibling != NULL)
                {
                    iterator->data = (void *) it->parent->next_sibling;
                    return result;
                }
                it = it->parent;
            }
            iterator->data = iterator->off;
        }
    }
    return result;
}

ei_widget_t* ei_it_widget_tree_get(ei_iterator_t *it)
{
    return (ei_widget_t *) it->data;
}