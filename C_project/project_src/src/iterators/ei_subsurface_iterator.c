#include "iterators/ei_subsurface_iterator.h"
#include "iterators/ei_iterator.h"
#include "ei_types.h"
#include "hw_interface.h"
#include "helpers/ei_memory.h"
#include "helpers/ei_draw_helpers.h"


typedef struct _ei_it_subsurface_user_data_t
{
	int x;
    int y;
    ei_surface_t surface;
    ei_rect_t *area;
} _ei_it_subsurface_user_data_t;


uint32_t* ei_it_subsurface_get(ei_iterator_t *it)
{
    return (uint32_t*) it->data;
}


ei_iterator_t ei_it_subsurface_begin(void* surface, ...)
{
    va_list argptr;
    va_start(argptr, surface);

	ei_rect_t * area = va_arg(argptr, ei_rect_t *);

	uint32_t *value = NULL;
	if (area->size.width != 0 && area->size.height != 0)
		value = offset_buffer_from_clipped((ei_surface_t) surface, area);
	if (value == NULL)
	{
		va_end(argptr);
		return (ei_iterator_t) {NULL, NULL, NULL, ei_it_subsurface_next, ei_it_subsurface_destroy};
	}

    ei_iterator_t it;

	it.data = value;
	it.off = NULL;
	it.next = ei_it_subsurface_next;
	it.destroyfunc = ei_it_subsurface_destroy;
	it.user_data = verified_calloc(1, sizeof(_ei_it_subsurface_user_data_t));
    ((_ei_it_subsurface_user_data_t *) it.user_data)->x = 0;
    ((_ei_it_subsurface_user_data_t *) it.user_data)->y = 0;
	((_ei_it_subsurface_user_data_t *) it.user_data)->surface = (ei_surface_t) surface;
	((_ei_it_subsurface_user_data_t *) it.user_data)->area = area;

    va_end(argptr);
    return it;
}



void* ei_it_subsurface_next(ei_iterator_t *it)
{
	// Grain sur les pixels : uint32_t et non uin8_t
	_ei_it_subsurface_user_data_t * user_data = (_ei_it_subsurface_user_data_t *) it->user_data;

	uint32_t *surface_first_pixel = (uint32_t *) hw_surface_get_buffer(user_data->surface);
	ei_rect_t surface_rect = hw_surface_get_rect(user_data->surface);
	size_t x_offset = user_data->area->top_left.x - surface_rect.top_left.x;
	size_t y_offset = user_data->area->top_left.y - surface_rect.top_left.y;

	int clipped_width = user_data->area->size.width;
	int clipped_height = user_data->area->size.height;

	
	void * result = it->data;
	if (user_data->x + 1 >= clipped_width)
	{
		user_data->x = 0;
		user_data->y += 1;
	}
	else
	{
		user_data->x += 1;
		it->data = (uint32_t *) it->data + 1;
		return result;
	}

	if (user_data->y >= clipped_height)
	{
        it->data = NULL;
		return NULL; // iterator get invalidated
	}
    it->data = (void *) (surface_first_pixel + ((y_offset + user_data->y) * surface_rect.size.width));
	// it->data += (x_offset << 2); // multiply by 4 cuz it->data is void*
	it->data = (uint32_t *) it->data + x_offset;

	return result;
}


void ei_it_subsurface_destroy(ei_iterator_t *it)
{
	free(it->user_data);
}