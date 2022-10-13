#include <assert.h>

#include "ei_draw.h"
#include "ei_utils.h"
#include "helpers/ei_math.h"
#include "helpers/ei_geometry_helpers.h"
#include "iterators/ei_iterator.h"
#include "iterators/ei_subsurface_iterator.h"

/**
 * @brief   Mix two pixels colors with an alpha parameter by computing the pondered mean of them.
 * Used to draw the widget with an effect of depth.
 * Uses the formula (3.1) of the subject.
 *
 * @param   dst_pixel   The destination pixel (first color value).
 * @param   src_pixel   The source pixel (second color value).
 * @param   alpha       The alpha value to use.
 *
 * @return  The mixed pixel, a value between 0 and 255 with src_pixel and dst_pixel mixed with the alpha value.
 *
 */
static uint8_t mix_with_alpha(uint32_t dst_pixel, uint32_t src_pixel, uint8_t src_alpha)
{
	return (uint8_t)((src_alpha * src_pixel + (255 - src_alpha) * dst_pixel) / 255);
}

uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color)
{
	int ir, ig, ib, ia;
	ir = ig = ib = ia = 0;
	hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

	uint32_t rgba_color = 0;
	rgba_color |= (((uint32_t)((uint8_t)~color.red)) << (ir * 8));
	rgba_color |= (((uint32_t)((uint8_t)~color.green)) << (ig * 8));
	rgba_color |= (((uint32_t)((uint8_t)~color.blue)) << (ib * 8));
	if (ia != -1)
		rgba_color |= (((uint32_t)((uint8_t)~color.alpha)) << (ia * 8));
	return ~rgba_color;
}

void ei_fill(ei_surface_t surface,
			 const ei_color_t *color,
			 const ei_rect_t *clipper)
{
	ei_rect_t surface_rect = hw_surface_get_rect(surface);

	ei_rect_t clipped = surface_rect;
	if (clipper != NULL)
		clipped = intersect_rect(&surface_rect, clipper);

	uint32_t cooked_color = 0xFFFFFFFF; // black opaque
	if (color != NULL)
		cooked_color = ei_map_rgba(surface, *color);
	ei_iterator_t pixel_it = ei_it_begin(subsurface, surface, &clipped);
	for (; ei_it_valid(&pixel_it); ei_it_next(&pixel_it))
	{
		*((uint32_t *)ei_it_get(&pixel_it)) = cooked_color;
	}
	ei_it_destroy(&pixel_it);
}

void ei_draw_text(ei_surface_t surface,
				  const ei_point_t *where,
				  const char *text,
				  ei_font_t font,
				  ei_color_t color,
				  const ei_rect_t *clipper)
{
#ifndef RELEASE
	assert(text != NULL && "text parameter cannot be NULL in ei_draw_text.\n");
#endif

	ei_surface_t text_surface = hw_text_create_surface(text, font, color);

	hw_surface_lock(text_surface);
	hw_surface_lock(surface);
	ei_size_t text_surface_size = hw_surface_get_size(text_surface);

	ei_rect_t text_surface_rect;
	if (where == NULL)
	{
		text_surface_rect = ei_rect(ei_point_zero(), text_surface_size);
	}
	else
	{
		text_surface_rect = ei_rect(*where, text_surface_size);
	}

	ei_rect_t clipped_text_rect;
	if (clipper != NULL)
	{
		clipped_text_rect = intersect_rect(&text_surface_rect, clipper);
	}
	else
	{
		clipped_text_rect.top_left = *where;
	}

	ei_rect_t clipped_src_rect = clipped_text_rect;
	clipped_src_rect.top_left = ei_point_zero();
	if (!ei_copy_surface(surface, &clipped_text_rect, text_surface, &clipped_src_rect, EI_TRUE))
	{
#ifndef RELEASE
		puts("copy_surface failed while drawing text (probably because frame->screen_location = {0,0,0,0})\n or because both sizes do not match.");
#endif
	}

	hw_surface_unlock(surface);
	hw_surface_unlock(text_surface);
	hw_surface_free(text_surface);

	// TODO devrions nous ei_app_invalidate_rect(&subsurface_rect); ?
}

int ei_copy_surface(ei_surface_t destination,
					const ei_rect_t *dst_rect,
					ei_surface_t source,
					const ei_rect_t *src_rect,
					ei_bool_t alpha)
{
	ei_rect_t dst_clipped = hw_surface_get_rect(destination);
	ei_rect_t src_clipped = hw_surface_get_rect(source);
	if (dst_rect != NULL)
		dst_clipped = intersect_rect(&dst_clipped, dst_rect);

	if (src_rect != NULL)
		src_clipped = intersect_rect(&src_clipped, src_rect);
	if (EI_COMPARE_SIZE(dst_clipped.size, !=, src_clipped.size))
		return 0;

	int ir, ig, ib, ia;
	ir = ig = ib = ia = 0;
	hw_surface_get_channel_indices(source, &ir, &ig, &ib, &ia);
	ei_iterator_t dst_it = ei_it_begin(subsurface, destination, &dst_clipped);
	ei_iterator_t src_it = ei_it_begin(subsurface, source, &src_clipped);

	for (; ei_it_valid(&dst_it); ei_it_next(&dst_it), ei_it_next(&src_it))
	{
#ifndef RELEASE
		assert((ia != -1 || !alpha) && "Alpha requested even though not supported by the source surface.");
#endif
		uint8_t *dst_pix = (uint8_t *)ei_it_get(&dst_it);
		uint8_t *src_pix = (uint8_t *)ei_it_get(&src_it);
		uint8_t alpha_value = *(src_pix + ABS(ia)); // Calling ABS just to prevent bad access to memory. In case of ia = -1, we cannot predict what is in memory right before the buffer.
		for (int i = 0; i < 4; i++)
		{
			if (i != ia)
				*(dst_pix + i) = (alpha) ? mix_with_alpha(*(dst_pix + i), *(src_pix + i), alpha_value) : *(src_pix + i);
			else
				*(dst_pix + i) = (alpha) ? *(dst_pix + i) : 0xff;
		}
	}
	ei_it_destroy(&dst_it);
	ei_it_destroy(&src_it);
	return 1;
}
