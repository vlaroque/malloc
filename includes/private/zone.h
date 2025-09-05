#ifndef ZONE_H
#define ZONE_H

#include <stddef.h>
#include "internal.h"

void *block_allocate(size_t requested_size, zone_type_t zone_type);
memory_zones_t *get_all_zones();
void del_memory_zone(mem_zone_t *mem_zone);
void *zone_init(zone_type_t zone_type, size_t requested_size, size_t aligned_requested_size);
mem_zone_t **_get_zone_by_type(zone_type_t zone_type);


#endif /*ZONE_H */
