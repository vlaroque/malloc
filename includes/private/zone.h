#ifndef ZONE_H
#define ZONE_H

#include <stddef.h>
#include "internal.h"

void *allocate_in_zone_array(size_t requested_size, zone_type_t zone_type);
memory_zones_t *get_all_zones();
void del_memory_zone(mem_zone_t *mem_zone);


#endif /*ZONE_H */
