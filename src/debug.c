
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "zone.h"
#include "printing.h"

void block_check(mem_block_t *block);

void dump_block(mem_block_t *block)
{
	str_out("[" COLOR_BOLD);
	putptr_out(block);
	str_out(COLOR_RESET "] " COLOR_YELLOW_FAINT "{" COLOR_RESET);
	DUMP_PTR(block, prev);
	str_out(", ");
	DUMP_PTR(block, next);
	str_out(", ");
	DUMP_PTR(block, belonging_zone);
	str_out(", ");

	str_out(COLOR_CYAN_BOLD "state" COLOR_RESET " = " COLOR_BLUE_BOLD);
	if ( block->state == BLOCK_STATE_FULL )
		str_out("BLOCK_STATE_FULL" COLOR_RESET);
	else if ( block->state == BLOCK_STATE_EMPTY )
		str_out("BLOCK_STATE_EMPTY" COLOR_RESET);
	str_out(", ");
	DUMP_INT(block, requested_size);
	str_out(", ");
	DUMP_INT(block, user_data_size);
	str_out(", ");
	DUMP_PTR(block, user_data);

	str_out(COLOR_YELLOW_FAINT "}" COLOR_RESET);
	block_check(block);
	str_out("\n");
}


void dump_zone(mem_zone_t *zone)
{
	if (zone == NULL)
		return;

	str_out(COLOR_YELLOW_FAINT " {" COLOR_RESET);
	DUMP_PTR(zone, prev);
	str_out(", ");
	DUMP_PTR(zone, next);
	str_out(", ");
	DUMP_PTR(zone, first_block);
	str_out(", ");
	DUMP_PTR(zone, last_block);
	str_out(", ");
	DUMP_INT(zone, zone_data_size);
	str_out(", ");
	DUMP_INT(zone, free_data_size);
	str_out(", ");
	DUMP_INT(zone, total_size);
	str_out(", ");
	DUMP_INT(zone, allocations_count);

	str_out(", type = ");
	if (zone->type == TINY)
		str_out("TINY");
	else if (zone->type == SMALL)
		str_out("SMALL");
	else if (zone->type == LARGE)
		str_out("LARGE");
	str_out("}\n");
	
	str_out(COLOR_WHITE "█ " COLOR_YELLOW_BOLD "█\n");


	mem_block_t *block = zone->first_block;

	int i = 0;
	while ( block != NULL )
	{
		str_out(COLOR_WHITE "█ " COLOR_YELLOW_BOLD "█ " COLOR_YELLOW "▓▓ BLOCK ");
		putnbr_out(i);
		str_out(" ▓▓ " COLOR_RESET );
		dump_block(block);
		block = block->next;
		i++;
	}
}


void block_check(mem_block_t *block)
{
	bool err = false;

	/*check position before*/

	if (block->prev == NULL)
	{
		if (block->belonging_zone->first_block != block)
		{
			str_out("ERROR - block->prev = NULL but not first block\n");
			err = true;
		}
	}
	else
	{
		if (block->prev->next != block)
		{
			str_out("ERROR - not the next of previous block\n");
			err = true;
		}
	}

	/*check position after*/

	if (block->next == NULL)
	{
		if (block->belonging_zone->last_block != block)
		{
			str_out("ERROR - block->next = NULL but not last block\n");
			err = true;
		}
	}
	else
	{
		if (block->next->prev != block)
		{
			str_out("ERROR - not the previous of next block\n");
			err = true;
		}
	}

	/* check the size of the block */

	if (block->state == BLOCK_STATE_FULL)
	{
		if ( (block->belonging_zone->type == TINY && block->user_data_size < MIN_TINY_SIZE)
		     || (block->belonging_zone->type == SMALL && block->user_data_size < MIN_SMALL_SIZE)
		     || (block->belonging_zone->type == LARGE && block->user_data_size < MIN_LARGE_SIZE))
		{
			str_out("ERROR - block too small for type\n");
			err = true;
		}

		if ( (block->belonging_zone->type == TINY && block->user_data_size > MAX_TINY_SIZE)
		     || (block->belonging_zone->type == SMALL && block->user_data_size > MAX_SMALL_SIZE))
		{
			str_out("ERROR - block too big for type\n");
			err = true;
		}
	}

	/* check the distante to next block */
	if (block->next != NULL && (size_t)( (buff_t)block->next - (buff_t)block ) != (block->user_data_size + BLOCK_HEADER_SIZE) )
	{
		str_out("ERROR - bad distance to next block\n");
		err = true;
	}

	/* check if two adjacent blocks are not free */
	if(block->next != NULL && block->state == BLOCK_STATE_EMPTY && block->next->state == BLOCK_STATE_EMPTY )
	{
		str_out("ERROR - two adjacent blocks are EMPTY\n");
		err = true;
	}

	/* check if sizes are consistent*/
	if (block->state == BLOCK_STATE_FULL && block->requested_size > block->user_data_size)
	{
		str_out("ERROR - requested size is bigger than data_size\n");
		err = true;
	}

	if (err)
	{
		str_out("    ERROR occured!");
	}
	else
	{
		str_out("    OK!");
	}
}

void zone_check(mem_zone_t *zone)
{
	if (zone->first_block->prev != NULL)
		str_out(COLOR_RED "    ERROR occured! First block prev not NULL");
	if (zone->last_block->next != NULL)
		str_out(COLOR_RED "    ERROR occured! Last block next not NULL");

	mem_zone_t **head = _get_zone_by_type(zone->type);
	if (zone->prev == NULL)
	{
		if ( *head != zone )
			str_out(COLOR_RED "    ERROR occured! zone prev is NULL but not at head");
	}
	else
	{
		if ( zone->prev->next != zone )
			str_out(COLOR_RED "    ERROR occured! zone prev next is not zone");
	}

	if ( zone->next != NULL && zone->next->prev != zone )
		str_out(COLOR_RED "    ERROR occured! zone next prev is not zone");

	int          free_data = 0;
	mem_block_t *block     = zone->first_block;

	while (block)
	{
		if ( block->state == BLOCK_STATE_EMPTY )
			free_data += block->user_data_size;
		block = block->next;
	}

	if ( free_data != (int)zone->free_data_size )
	{
		str_out(COLOR_RED "    ERROR occured! Free data inconsistent!");
		exit(1);
	}
}

void dump_globals()
{
	DUMP_VAR(TINY_SIZE);
	DUMP_VAR(SMALL_SIZE);
	DUMP_VAR(MIN_ALLOCS_PER_ZONE);
	DUMP_VAR(ALIGNMENT);
	DUMP_VAR(MIN_TINY_SIZE);
	DUMP_VAR(MAX_TINY_SIZE);
	DUMP_VAR(MIN_SMALL_SIZE);
	DUMP_VAR(MAX_SMALL_SIZE);
	DUMP_VAR(MIN_LARGE_SIZE);
	DUMP_VAR(BLOCK_HEADER_SIZE);
	DUMP_VAR(MAX_TINY_BLOCK_SIZE);
	DUMP_VAR(MAX_SMALL_BLOCK_SIZE);
	DUMP_VAR(ZONE_HEADER_SIZE);
	DUMP_VAR(TINY_ZONE_SIZE);
	DUMP_VAR(SMALL_ZONE_SIZE);
}


void dump_zone_list(mem_zone_t *zone)
{
	int i = 0;
	while (zone != NULL)
	{
		str_out("\n" COLOR_WHITE "█ " COLOR_YELLOW_BOLD "██ ZONE ");
		putnbr_out(i);
		str_out(" ██" COLOR_RESET);
		dump_zone(zone);
		zone_check(zone);
		i++;
		zone = zone->next;
	}
}

void pretty_dump_memory()
{
	static bool dump = true;

	if (dump)
	{
		dump = false;
		dump_globals();
	}

	str_out(COLOR_BLUE_BOLD "\n████████████████████████████████████████████████████████████████████████████████\n" COLOR_RESET);

	memory_zones_t *zones = get_all_zones();
	str_out("████ TINY ZONES ████\n" COLOR_WHITE "█");
	dump_zone_list(zones->tiny_zones);
	str_out("\n████ SMALL ZONES ████\n");
	dump_zone_list(zones->small_zones);
	str_out("\n████ LARGE ZONES ████\n");
	dump_zone_list(zones->large_zones);
}

size_t basic_block_dump(mem_block_t *block)
{
	size_t block_size = block->user_data_size + BLOCK_HEADER_SIZE;
	putptr_out((buff_t*)block->user_data);
	str_out(" - ");
	putptr_out(((buff_t*)block) + block_size );
	str_out(" : ");
	putnbr_out((int)block->user_data_size);
	str_out(" bytes\n");

	return block->user_data_size;
}

size_t basic_zone_dump(mem_zone_t *zone)
{
	size_t total_size = 0;

	if ( zone->type == TINY )
		str_out("TINY : ");
	else if ( zone->type == SMALL )
		str_out("SMALL : ");
	else
		str_out("LARGE : ");

	putptr_out((buff_t*)zone);

	str_out("\n");

	mem_block_t *block = zone->first_block;
	while (block)
	{
		if (block->state != BLOCK_STATE_EMPTY)
		{
			total_size += basic_block_dump(block);
		}

		block = block->next;
	}

	return total_size;
}

size_t basic_zones_dump(mem_zone_t *head_zone)
{
	mem_zone_t *zone = head_zone;
	size_t total_size = 0;

	while( zone )
	{
		total_size += basic_zone_dump(zone);
		zone = zone->next;
	}
	return total_size;
}

void dump_memory()
{
	size_t total_size = 0;

	memory_zones_t *zones = get_all_zones();
	total_size += basic_zones_dump(zones->tiny_zones);
	total_size += basic_zones_dump(zones->small_zones);
	total_size += basic_zones_dump(zones->large_zones);
	str_out("Total : ");
	putnbr_out(total_size);
	str_out(" bytes\n");

}
