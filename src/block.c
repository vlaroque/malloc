/*****************************************************************************/
/*                                                                           */
/*                                                🭈🭆🭂███🭞🭜🭘  ███🭞🭜🭘██████    */
/*   block.c                                   🭈🭆🭂███🭞🭜🭘     🭞🭜🭘   ██████    */
/*                                          🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘    */
/*   By: vlaroque                        🭈🭆🭂███🭞🭜🭘           🭈🭆🭂███🭞🭜🭘       */
/*       vlaroque@student.42.fr          ██████████████████  ██████   🭈🭆🭂    */
/*                                       ██████████████████  ██████🭈🭆🭂███.fr */
/*                                                   ██████   🬚🬢🬞🬋🬏🬚🬢🬦🬞🬋🬃    */
/*                                                   ██████   🬕🬀▐🬂▌🬕🬣▐🬞🬰🬃    */
/*                                                                           */
/*****************************************************************************/

#include "block.h"
#include "zone.h"
#include "internal.h"
#include <assert.h>

void *init_block(mem_zone_t *zone, mem_block_t *block_to_init, mem_block_t *previous_block, mem_block_t *next_block, size_t requested_size, size_t user_data_size, block_state_t state)
{
	assert(zone != NULL);
	assert(block_to_init != NULL);
	assert(user_data_size == ALIGN(user_data_size));

	block_to_init->prev           = previous_block;
	block_to_init->next           = next_block;
	block_to_init->state          = state;
	block_to_init->requested_size = requested_size;
	block_to_init->user_data_size = user_data_size;
	block_to_init->user_data      = (void *)( (buff_t)block_to_init + BLOCK_HEADER_SIZE );
	block_to_init->belonging_zone = zone;

	if ( block_to_init->state == BLOCK_STATE_EMPTY )
		zone->free_data_size -= BLOCK_HEADER_SIZE;
	else if ( block_to_init->state == BLOCK_STATE_FULL )
		zone->free_data_size -= (BLOCK_HEADER_SIZE + user_data_size);

	return block_to_init->user_data;
}


bool block_is_empty_and_can_hold(mem_block_t *mem_block, size_t user_data_size)
{
	return (mem_block->state == BLOCK_STATE_EMPTY
	        && mem_block->user_data_size >= user_data_size );
}

size_t get_zone_min_size_block(mem_zone_t *zone)
{
	switch (zone->type)
	{
		case TINY:
			return MIN_TINY_SIZE;
		case SMALL:
			return MIN_SMALL_SIZE;
		case LARGE:
			return MIN_LARGE_SIZE;
	}

	return MIN_LARGE_SIZE;
}

void *occupy_empty_block(mem_zone_t *zone, mem_block_t *empty_block, size_t requested_size, size_t block_data_size)
{
	assert(zone != NULL);
	assert(empty_block != NULL);

	size_t min_size = get_zone_min_size_block(zone);

	zone->free_data_size    += BLOCK_HEADER_SIZE;
	zone->allocations_count += 1;

	/* here we decide if we split the empty block in two or not */
	if ( empty_block->user_data_size >= block_data_size + BLOCK_HEADER_SIZE + min_size)
	{
		size_t new_block_size = BLOCK_HEADER_SIZE + block_data_size;

		mem_block_t *middle_block = (mem_block_t *)((buff_t)empty_block + new_block_size);

		init_block(zone,
		           middle_block,
		           empty_block,
		           empty_block->next,
		           0,
		           empty_block->user_data_size - new_block_size,
		           BLOCK_STATE_EMPTY);

		if ( empty_block->next != NULL )
		{
			empty_block->next->prev = middle_block;
		}
		else
		{
			zone->last_block = middle_block;
		}

		return ( init_block(zone,
		                    empty_block,
		                    empty_block->prev,
		                    middle_block,
		                    requested_size,
		                    block_data_size,
		                    BLOCK_STATE_FULL) );
	}
	else
	{
		return ( init_block(zone,
		                    empty_block,
		                    empty_block->prev,
		                    empty_block->next,
		                    requested_size,
		                    block_data_size,
		                    BLOCK_STATE_FULL) );
	}

}

mem_block_t *find_in_zone_block_big_enough(mem_zone_t *zone, size_t block_data_size)
{
	assert(zone != NULL);

	/* first block is always init, at least empty */
	mem_block_t *cur_block = zone->first_block;

	while ( cur_block )
	{
		if ( block_is_empty_and_can_hold(cur_block, block_data_size) )
		{
			return cur_block;
		}

		cur_block = cur_block->next;
	}

	return NULL;
}

void fuze_blocks(mem_block_t *first_block, mem_block_t *second_block)
{
	assert(first_block->state == BLOCK_STATE_EMPTY);
	assert(second_block->state == BLOCK_STATE_EMPTY);

	first_block->next = second_block->next;
	mem_zone_t *zone = first_block->belonging_zone;
	
	if ( first_block->next == NULL )
	{
		assert(zone->last_block == second_block);
		zone->last_block = first_block;
	}
	else
	{
		first_block->next->prev = first_block;
	}
	first_block->user_data_size += BLOCK_HEADER_SIZE + second_block->user_data_size;
}

void block_deallocate(mem_block_t *block)
{
	mem_zone_t *zone = block->belonging_zone;

	if ( zone->type == LARGE )
	{
		del_memory_zone(zone);
		return;
	}

	block->state         =  BLOCK_STATE_EMPTY;
	zone->free_data_size += block->user_data_size;
	zone->allocations_count -= 1;

	if ( block->next != NULL && block->next->state == BLOCK_STATE_EMPTY )
	{
		zone->free_data_size += BLOCK_HEADER_SIZE;
		fuze_blocks(block, block->next);
	}

	if ( block->prev != NULL && block->prev->state == BLOCK_STATE_EMPTY)
	{
		zone->free_data_size += BLOCK_HEADER_SIZE;
		fuze_blocks(block->prev, block);
	}
}

void *block_allocate(size_t requested_size, zone_type_t zone_type)
{
	mem_zone_t **mem_zone_ptr     = _get_zone_by_type(zone_type);
	size_t aligned_requested_size = ALIGN(requested_size);

	if ( mem_zone_ptr == NULL )
		return NULL;

	if ( *mem_zone_ptr == NULL )
	{
		return zone_init(zone_type, requested_size, aligned_requested_size);
	}

	mem_zone_t *zone = *mem_zone_ptr;
	while ( zone )
	{
		mem_block_t *empty_block = find_in_zone_block_big_enough(zone, aligned_requested_size);

		if ( empty_block != NULL)
		{
			return occupy_empty_block(zone, empty_block, requested_size, aligned_requested_size);
		}

		if ( zone->next == NULL )
		{
			return zone_init(zone_type, requested_size, aligned_requested_size);
		}

		zone = zone->next;
	}

	return NULL;
}
