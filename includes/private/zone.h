#ifndef ZONE_H
#define ZONE_H

#include <stddef.h>
#include "internal.h"

zone_header_t *mem_zone_get_header(mem_zone_t zone);
void *allocate_in_zone_array(size_t requested_size, zone_type_t zone_type);

#endif /*ZONE_H */
