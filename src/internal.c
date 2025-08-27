#include <stddef.h>
#include <unistd.h>

size_t get_page_size(void)
{
#ifdef __APPLE__
    return getpagesize();
#else
    return sysconf(_SC_PAGESIZE);
#endif
}
