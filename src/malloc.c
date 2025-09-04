#include "libft_malloc.h"
#include <stddef.h>
#include <libft.h>
#include "zone.h"
#include "block.h"
#include "internal.h"


void *malloc(size_t size)
{
	void *ret = NULL;

	if ( size == 0 )
		return ret;

	if ( size <= MAX_TINY_SIZE )
	{
		ret = allocate_in_zone_array(size, TINY);
	}
	else if ( size <= MAX_SMALL_SIZE )
	{
		ret = allocate_in_zone_array(size, SMALL);
	}
	else
	{
		ret = allocate_in_zone_array(size, LARGE);
	}

	return ret;
}

void free(void *ptr)
{
	deallocate_block( (mem_block_t*)((buff_t)ptr - BLOCK_HEADER_SIZE ) );
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

	mem_block_t *block = (mem_block_t *)((buff_t)ptr - BLOCK_HEADER_SIZE);
	size = ALIGN(size);

	if (block->user_data_size >= size )
	{
		return ptr;
	}

	void *new_ptr = malloc(size);

	if (new_ptr == NULL)
		return new_ptr;

	ft_memcpy(new_ptr, ptr, block->requested_size);

	free(ptr);

	return NULL;
}

void show_alloc_mem(void)
{
	dump_memory();

	return;
}
