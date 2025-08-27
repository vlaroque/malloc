#include "libft_malloc.h"
#include <stddef.h>

void *malloc(size_t size)
{
	size += 1;
	return NULL;
}

void free(void *ptr)
{
	ptr += 1;
}

void *realloc(void *ptr, size_t size)
{
	if ( size == 0 )
		free(ptr);

	return NULL;
}

void show_alloc_mem(void)
{
}
