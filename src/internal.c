#include <stddef.h>
#include <unistd.h>
#include "internal.h"

size_t get_page_size(void)
{
#ifdef __APPLE__
    return getpagesize();
#else
    return sysconf(_SC_PAGESIZE);
#endif
}

size_t make_page_size_multiple(size_t size)
{
	size_t page_size = get_page_size();

	if ( size % page_size != 0 )
		size = ( ( size / page_size) + 1 ) * page_size;

	return size;
}
