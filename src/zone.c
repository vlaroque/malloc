#include "internal.h"
#include "zone.h"
#include "block.h"
#include <sys/mman.h>
#include <assert.h>


/* HERE lies the globals */
typedef struct memory_zones_s
{
	mem_zone_t tiny_zones;
	mem_zone_t small_zones;
	mem_zone_t large_zones;
} memory_zones_t;

memory_zones_t g_malloc_zones = { NULL };
/* End of globals */


zone_header_t *mem_zone_get_header(mem_zone_t zone)
{
	return (zone_header_t *)zone;
}

mem_zone_t mem_zone_get_next(mem_zone_t zone)
{
	assert(zone != NULL);
	return( (mem_zone_t)(mem_zone_get_header(zone)->next) );
}

/*!
 * \brief get the size of the whole allocated zone
 *
 * \param[in] zone the zone ptr
 *
 * \retval the size of the zone
 */
size_t mem_zone_get_size(mem_zone_t zone)
{
	assert(zone != NULL);
	return( (mem_zone_get_header(zone)->zone_data_size) + ZONE_HEADER_SIZE );
}

/*!
 * \brief allocate a new momory zone for containing at least size memory
 *
 * \param[in] size the size needed to be utilised
 *
 * \return a pointer to the zone, NULL otherwise
 */
mem_zone_t new_memory_zone(size_t mem_size, zone_type_t zone_type)
{
	size_t data_size = ALIGN(mem_size);
	size_t total_size = make_page_size_multiple(ZONE_HEADER_SIZE + data_size);

	void *mem = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if ( mem == MAP_FAILED )
		return NULL;

	zone_header_t *zone_header = (zone_header_t *) mem;
	zone_header->fist_block = (mem_block_t *)(zone_header + ZONE_HEADER_SIZE);
	zone_header->last_block = (mem_block_t *)(zone_header + ZONE_HEADER_SIZE);
	zone_header->zone_data_size = data_size;
	zone_header->free_data_size = data_size; /* TODO maybe remove it */
	zone_header->allocations_count = 0;
	zone_header->type = zone_type;

	mem_block_t *first_block = zone_header->fist_block;
	first_empty_block((mem_zone_t )zone_header, first_block, data_size);

	return (mem_zone_t) mem;
}

static mem_zone_t *_get_zone_by_type(zone_type_t zone_type)
{
	if (zone_type == TINY )
		return &g_malloc_zones.tiny_zones;
	else if (zone_type == SMALL )
		return &g_malloc_zones.small_zones;
	else if (zone_type == LARGE )
		return &g_malloc_zones.large_zones;
	else
		return NULL;
}

static size_t _get_zone_size_by_type(zone_type_t zone_type, size_t data_size)
{
	switch ( zone_type )
	{
		case TINY:
			return TINY_ZONE_SIZE;
			break;
		case SMALL:
			return SMALL_ZONE_SIZE;
			break;
		case LARGE:
			return ( ZONE_HEADER_SIZE + BLOCK_HEADER_SIZE + ALIGN(data_size) );
	}

	return 0;
}

void *allocate_in_zone_array(size_t requested_size, zone_type_t zone_type)
{
	mem_zone_t *mem_zone_ptr = _get_zone_by_type(zone_type);

	if ( mem_zone_ptr == NULL )
		return NULL;

	if ( *mem_zone_ptr == NULL )
	{
		size_t     zone_size = _get_zone_size_by_type(zone_type, requested_size);
		mem_zone_t new       = new_memory_zone(zone_size, zone_type);
		*mem_zone_ptr        = new;

		return try_allocate_in_zone(new, requested_size);
	}

	while ( *mem_zone_ptr )
	{
		void *allocated_ptr = try_allocate_in_zone(*mem_zone_ptr, requested_size);

		if ( allocated_ptr != NULL)
			return allocated_ptr;

		if ( mem_zone_get_next(*mem_zone_ptr) == NULL )
			break;

		*mem_zone_ptr = mem_zone_get_next(*mem_zone_ptr);
	}

	/* code dupliqué à mettre à jour */
	size_t zone_size = _get_zone_size_by_type(zone_type, requested_size);
	mem_zone_t new   = new_memory_zone(zone_size, zone_type);
	*mem_zone_ptr = new;

	mem_zone_get_header(new)->prev = *mem_zone_ptr;
	
	return try_allocate_in_zone(new, requested_size);
}

void del_memory_zone(mem_zone_t mem_zone)
{
	assert(mem_zone != NULL);

	munmap((void*)mem_zone, mem_zone_get_size(mem_zone));

	return;
}

/*
void clear_memory_zone(mem_zone_t mem_zone)
{
	zone_type_t type = mem_zone;
}
*/

