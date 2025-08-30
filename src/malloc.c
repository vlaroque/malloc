#include "libft_malloc.h"
#include <stddef.h>
#include "zone.h"
#include "internal.h"
#include <assert.h>
#include <unistd.h>
#include <string.h>

static void to_stdout(char *str)
{
	assert(str != NULL);

	ssize_t ignored = write(STDOUT_FILENO, str, strlen(str));
	ignored = write(STDERR_FILENO, str, strlen(str));
	(void)ignored;
}

void *malloc(size_t size)
{
	to_stdout("entering malloc\n");

	size = ALIGN(size);

	if ( size <= MAX_TINY_SIZE )
		return allocate_in_zone_array(size, TINY);
	else if ( size <= MAX_SMALL_SIZE )
		return allocate_in_zone_array(size, SMALL);
	else
		return allocate_in_zone_array(size, LARGE);

	return NULL;
}

void free(void *ptr)
{
	(ptr += 1);
	return;
}

void *realloc(void *ptr, size_t size)
{
	if ( size == 0 )
		free(ptr);

	return NULL;
}

void show_alloc_mem(void)
{
	return;
}
