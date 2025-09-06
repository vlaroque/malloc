#ifndef LIBFT_MALLOC_H
#define LIBFT_MALLOC_H

#include <stddef.h>

/* Function prototypes */
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void show_alloc_mem(void);

/* Bonus part */
void pretty_show_alloc_mem(void);

#endif
