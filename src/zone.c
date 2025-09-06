/*****************************************************************************/
/*                                                                           */
/*                                                🭈🭆🭂███🭞🭜🭘  ███🭞🭜🭘██████    */
/*   zone.c                                    🭈🭆🭂███🭞🭜🭘     🭞🭜🭘   ██████    */
/*                                          🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘    */
/*   By: vlaroque                        🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘       */
/*       vlaroque@student.42.fr          ██████████████████  ██████   🭈🭆🭂    */
/*                                       ██████████████████  ██████🭈🭆🭂███.fr */
/*                                                   ██████   🬚🬢🬞🬋🬏🬚🬢🬦🬞🬋🬃    */
/*                                                   ██████   🬕🬀▐🬂▌🬕🬣▐🬞🬰🬃    */
/*                                                                           */
/*****************************************************************************/

#include "internal.h"
#include "zone.h"
#include "block.h"
#include "printing.h"
#include <sys/mman.h>
#include <assert.h>

memory_zones_t g_malloc_zones = { NULL };
/* End of globals */


/*!
 * \brief get the size of the whole allocated zone
 *
 * \param[in] zone the zone ptr
 *
 * \retval the size of the zone
 */
size_t mem_zone_get_size(mem_zone_t *zone)
{
	assert(zone != NULL);
	return( zone->zone_data_size + ZONE_HEADER_SIZE );
}

mem_zone_t **_get_zone_by_type(zone_type_t zone_type)
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

size_t _get_zone_size_by_type(zone_type_t zone_type, size_t data_size)
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

/*!
 * \brief allocate a new momory zone for containing at least size memory
 *
 * \param[in] size the size needed to be utilised
 *
 * \return a pointer to the zone, NULL otherwise
 */
mem_zone_t *new_memory_zone(size_t mem_size, zone_type_t zone_type)
{
	size_t data_size = ALIGN(mem_size);
	size_t total_size = make_page_size_multiple(data_size);

	void *mem = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if ( mem == MAP_FAILED )
	{
		str_out("ERROR MMAP\n");
		return NULL;
	}

	mem_zone_t *new_zone = (mem_zone_t *) mem;

	new_zone->first_block       = (mem_block_t *)((buff_t)new_zone + ZONE_HEADER_SIZE);
	new_zone->last_block        = new_zone->first_block;
	new_zone->zone_data_size    = data_size;
	new_zone->free_data_size    = data_size - ZONE_HEADER_SIZE; /* TODO maybe remove it */
	new_zone->total_size        = total_size; /* TODO maybe remove it */
	new_zone->allocations_count = 0;
	new_zone->type              = zone_type;
	new_zone->prev              = NULL;
	new_zone->next              = NULL;

	init_block(new_zone,
	           new_zone->first_block,
	           NULL,
	           NULL,
	           new_zone->free_data_size - BLOCK_HEADER_SIZE,
	           new_zone->free_data_size - BLOCK_HEADER_SIZE,
	           BLOCK_STATE_EMPTY);

	mem_zone_t **mem_zone_ptr     = _get_zone_by_type(zone_type);

	if ( *mem_zone_ptr == NULL )
		*mem_zone_ptr = new_zone;
	else
	{
		mem_zone_t *zone = *mem_zone_ptr;
		while ( zone )
		{
			if ( zone->next == NULL)
			{
				zone->next      = new_zone;
				new_zone->prev  = zone;
				break;
			}
			zone = zone->next;
		}
	}
	return new_zone;
}

void *zone_init(zone_type_t zone_type, size_t requested_size, size_t aligned_requested_size)
{
	size_t      zone_size = _get_zone_size_by_type(zone_type, aligned_requested_size);
	mem_zone_t *new_zone  = new_memory_zone(zone_size, zone_type);

	if ( new_zone == NULL )
		return NULL;

	return occupy_empty_block(new_zone, new_zone->first_block, requested_size, aligned_requested_size);
}

void del_memory_zone(mem_zone_t *mem_zone)
{
	assert(mem_zone != NULL);

	mem_zone_t *prev_zone = mem_zone->prev;
	mem_zone_t *next_zone = mem_zone->next;

	if ( prev_zone != NULL )
	{
		prev_zone->next = next_zone;
	}
	else
	{
		mem_zone_t **head = _get_zone_by_type(mem_zone->type);
		*head = next_zone;
	}

	if ( next_zone != NULL )
	{
		next_zone->prev = prev_zone;
	}

	assert( munmap((void*)mem_zone, mem_zone->total_size) == 0 );

	return;
}

memory_zones_t *get_all_zones()
{
	return &g_malloc_zones;
}
