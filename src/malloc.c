/*****************************************************************************/
/*                                                                           */
/*                                                🭈🭆🭂███🭞🭜🭘  ███🭞🭜🭘██████    */
/*   malloc.c                                  🭈🭆🭂███🭞🭜🭘     🭞🭜🭘   ██████    */
/*                                          🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘    */
/*   By: vlaroque                        🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘       */
/*       vlaroque@student.42.fr          ██████████████████  ██████   🭈🭆🭂    */
/*                                       ██████████████████  ██████🭈🭆🭂███.fr */
/*                                                   ██████   🬚🬢🬞🬋🬏🬚🬢🬦🬞🬋🬃    */
/*                                                   ██████   🬕🬀▐🬂▌🬕🬣▐🬞🬰🬃    */
/*                                                                           */
/*****************************************************************************/

#include "libft_malloc.h"
#include <stddef.h>
#include <libft.h>
#include "zone.h"
#include "block.h"
#include "internal.h"
#include <pthread.h>

static pthread_mutex_t malloc_lock = PTHREAD_MUTEX_INITIALIZER;

zone_type_t get_zone_type_from_size(size_t size)
{
	if ( size <= MAX_TINY_SIZE )
		return TINY;
	else if ( size <= MAX_SMALL_SIZE )
		return SMALL;
	else
		return LARGE;
}

void *malloc(size_t size)
{
	void *ret = NULL;

	if ( size == 0 )
		return NULL;

	pthread_mutex_lock(&malloc_lock);
	ret = block_allocate(size, get_zone_type_from_size(size));
	pthread_mutex_unlock(&malloc_lock);

	return ret;
}

void free(void *ptr)
{
	if (ptr == NULL)
		return;

	pthread_mutex_lock(&malloc_lock);
	block_deallocate( (mem_block_t*)((buff_t)ptr - BLOCK_HEADER_SIZE ) );
	pthread_mutex_unlock(&malloc_lock);

	return;
}

void *realloc(void *ptr, size_t size)
{
	if ( ptr == NULL)
	{
		return malloc(size);
	}

	if ( size == 0 )
	{
		free(ptr);
		return NULL;
	}

	pthread_mutex_lock(&malloc_lock);

	mem_block_t *block = (mem_block_t *)((buff_t)ptr - BLOCK_HEADER_SIZE);

	if (block->user_data_size >= ALIGN(size) )
	{
		zone_type_t new_zone_type = get_zone_type_from_size(size);
		zone_type_t current_zone_type = block->belonging_zone->type;

		if ( new_zone_type == current_zone_type )
		{
			block->requested_size = size;
			pthread_mutex_unlock(&malloc_lock);
			return ptr;
		}
	}

	void *new_ptr = block_allocate(size, get_zone_type_from_size(size));
	
	if (new_ptr == NULL)
	{
		pthread_mutex_unlock(&malloc_lock);
		return NULL;
	}

	size_t copy_size = (block->requested_size < size) ? block->requested_size : size;

	ft_memcpy(new_ptr, ptr, copy_size);

	block_deallocate( block );

	pthread_mutex_unlock(&malloc_lock);

	return new_ptr;
}

void show_alloc_mem(void)
{
	pthread_mutex_lock(&malloc_lock);
	dump_memory();
	pthread_mutex_unlock(&malloc_lock);
}

void pretty_show_alloc_mem(void)
{
	pthread_mutex_lock(&malloc_lock);
	pretty_dump_memory();
	pthread_mutex_unlock(&malloc_lock);
}
