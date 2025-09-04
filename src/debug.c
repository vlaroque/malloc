
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "zone.h"

#define COLOR_BLACK   "\x1b[30m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_YELLOW_BOLD  "\x1b[1;33m"
#define COLOR_YELLOW_FAINT  "\x1b[2;33m"
#define COLOR_BLUE          "\x1b[34m"
#define COLOR_BLUE_BOLD          "\x1b[1;34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_CYAN_BOLD    "\x1b[1;36m"
#define COLOR_WHITE   "\x1b[37m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BOLD    "\x1b[1m"

void to_stdout(char *str)
{
	assert(str != NULL);

	ssize_t ignored = write(STDOUT_FILENO, str, strlen(str));
	(void)ignored;
}

void tiny_putnbr(int n)
{
	static char buf[32]; // big enough for int
	int i = 30;          // index to fill backwards
	int neg = 0;

	if (n == 0)
	{
		ssize_t ignored = write(STDOUT_FILENO, "0", 1);
		(void) ignored;
		return;
	}

	if (n < 0) {
		neg = 1;
		// careful with INT_MIN: use long
		long ln = -(long)n;
		while (ln > 0 && i >= 0) {
			buf[i--] = '0' + (ln % 10);
			ln /= 10;
		}
	}
	else
	{
		while (n > 0 && i >= 0) {
			buf[i--] = '0' + (n % 10);
			n /= 10;
		}
	}

	if (neg)
		buf[i--] = '-';

	ssize_t ignored = write(STDOUT_FILENO, buf + i + 1, 30 - i);
	(void)ignored;

}

void tiny_puthex(unsigned long n)
{
	static char buf[32]; // plenty for 64-bit hex (max 16 digits + "0x")
	const char *digits = "0123456789ABCDEF";
	int i = 30;

	if (n == 0) {
		ssize_t ignored = write(STDOUT_FILENO, "0x0", 3);
		(void)ignored;

		return;
	}

	while (n > 0 && i >= 0) {
		buf[i--] = digits[n & 0xF]; // n % 16
		n >>= 4;                    // n /= 16
	}

	buf[i--] = 'x';
	buf[i--] = '0';

	ssize_t ignored = write(STDOUT_FILENO, buf + i + 1, 30 - i);
	(void)ignored;
}

void tiny_putptr(void* n)
{
	tiny_puthex((unsigned long)n);
}

#define DUMP_PTR(obj, field) \
	do { \
		to_stdout(COLOR_CYAN_BOLD #field COLOR_RESET " = " COLOR_BLUE);\
		tiny_putptr(obj->field);\
		if (obj->field == NULL)\
		{\
			to_stdout("           ");\
		}\
		to_stdout(COLOR_RESET);\
	} while (0)

#define DUMP_INT(obj, field) \
	do { \
		to_stdout(COLOR_CYAN_BOLD #field COLOR_RESET " = " COLOR_BLUE);\
		tiny_putnbr(obj->field);\
		to_stdout(COLOR_RESET);\
	} while (0)

void block_check(mem_block_t *block);

void dump_block(mem_block_t *block)
{
	to_stdout("[" COLOR_BOLD);
	tiny_putptr(block);
	to_stdout(COLOR_RESET "] " COLOR_YELLOW_FAINT "{" COLOR_RESET);
	DUMP_PTR(block, prev);
	to_stdout(", ");
	DUMP_PTR(block, next);
	to_stdout(", ");
	DUMP_PTR(block, belonging_zone);
	to_stdout(", ");

	to_stdout(COLOR_CYAN_BOLD "state" COLOR_RESET " = " COLOR_BLUE_BOLD);
	if ( block->state == BLOCK_STATE_FULL )
		to_stdout("BLOCK_STATE_FULL" COLOR_RESET);
	else if ( block->state == BLOCK_STATE_EMPTY )
		to_stdout("BLOCK_STATE_EMPTY" COLOR_RESET);
	to_stdout(", ");
	DUMP_INT(block, requested_size);
	to_stdout(", ");
	DUMP_INT(block, user_data_size);
	to_stdout(", ");
	DUMP_PTR(block, user_data);

	to_stdout(COLOR_YELLOW_FAINT "}" COLOR_RESET);
	block_check(block);
	to_stdout("\n");
}


void dump_zone(mem_zone_t *zone)
{
	if (zone == NULL)
		return;

	to_stdout(COLOR_YELLOW_FAINT " {" COLOR_RESET);
	DUMP_PTR(zone, prev);
	to_stdout(", ");
	DUMP_PTR(zone, next);
	to_stdout(", ");
	DUMP_PTR(zone, fist_block);
	to_stdout(", ");
	DUMP_PTR(zone, last_block);
	to_stdout(", ");
	DUMP_INT(zone, zone_data_size);
	to_stdout(", ");
	DUMP_INT(zone, free_data_size);
	to_stdout(", ");
	DUMP_INT(zone, total_size);
	to_stdout(", ");
	DUMP_INT(zone, allocations_count);

	to_stdout(", type = ");
	if (zone->type == TINY)
		to_stdout("TINY");
	else if (zone->type == SMALL)
		to_stdout("SMALL");
	else if (zone->type == LARGE)
		to_stdout("LARGE");
	to_stdout("}\n");

	mem_block_t *block = zone->fist_block;

	int i = 0;
	while ( block != NULL )
	{
		to_stdout(COLOR_YELLOW "▓▓ BLOCK ");
		tiny_putnbr(i);
		to_stdout(" ▓▓ " COLOR_RESET );
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
		if (block->belonging_zone->fist_block != block)
		{
			to_stdout("ERROR - block->prev = NULL but not first block\n");
			err = true;
		}
	}
	else
	{
		if (block->prev->next != block)
		{
			to_stdout("ERROR - not the next of previous block\n");
			err = true;
		}
	}

	/*check position after*/

	if (block->next == NULL)
	{
		if (block->belonging_zone->last_block != block)
		{
			to_stdout("ERROR - block->next = NULL but not last block\n");
			err = true;
		}
	}
	else
	{
		if (block->next->prev != block)
		{
			to_stdout("ERROR - not the previous of next block\n");
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
			to_stdout("ERROR - block too small for type\n");
			err = true;
		}

		if ( (block->belonging_zone->type == TINY && block->user_data_size > MAX_TINY_SIZE)
		     || (block->belonging_zone->type == SMALL && block->user_data_size > MAX_SMALL_SIZE))
		{
			to_stdout("ERROR - block too big for type\n");
			err = true;
		}
	}

	/* check the distante to next block */
	if (block->next != NULL && (size_t)( (buff_t)block->next - (buff_t)block ) != (block->user_data_size + BLOCK_HEADER_SIZE) )
	{
		to_stdout("ERROR - bad distance to next block\n");
		err = true;
	}

	/* check if two adjacent blocks are not free */
	if(block->next != NULL && block->state == BLOCK_STATE_EMPTY && block->next->state == BLOCK_STATE_EMPTY )
	{
		to_stdout("ERROR - two adjacent blocks are EMPTY\n");
		err = true;
	}

	if (err)
	{
		to_stdout("    ERROR occured!");
	}
	else
	{
		to_stdout("    OK!");
	}
}

void zone_check(mem_zone_t *zone)
{
	if (zone->fist_block->prev != NULL)
		to_stdout(COLOR_RED "    ERROR occured! First block prev not NULL");
	if (zone->last_block->next != NULL)
		to_stdout(COLOR_RED "    ERROR occured! Last block next not NULL");

	int free_data = 0;
	
	mem_block_t *block = zone->fist_block;

	while (block)
	{
		if ( block->state == BLOCK_STATE_EMPTY )
			free_data += block->user_data_size;
		block = block->next;
	}

	if ( free_data != (int)zone->free_data_size )
	{
		to_stdout(COLOR_RED "    ERROR occured! Free data inconsistent!");
		exit(1);
	}
}

#define DUMP_VAR(var) do\
	{\
		to_stdout(#var " = ");\
		tiny_putnbr(var);\
		to_stdout("\n");\
	} while(0)

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
		to_stdout("\n" COLOR_YELLOW_BOLD "██ ");
		to_stdout("ZONE ");
		tiny_putnbr(i);
		to_stdout(" ██" COLOR_RESET);
		dump_zone(zone);
		zone_check(zone);
		i++;
		zone = zone->next;
	}
}

void dump_memory()
{
	static bool dump = true;

	if (dump)
	{
		dump = false;
		dump_globals();
	}

	to_stdout(COLOR_BLUE_BOLD "\n████████████████████████████████████████████████████████████████████████████████\n" COLOR_RESET);

	memory_zones_t *zones = get_all_zones();
	to_stdout("████ TINY ZONES ████\n");
	dump_zone_list(zones->tiny_zones);
	to_stdout("\n████ SMALL ZONES ████\n");
	dump_zone_list(zones->small_zones);
	to_stdout("\n████ LARGE ZONES ████\n");
	dump_zone_list(zones->large_zones);
}

