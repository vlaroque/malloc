#ifndef INTERNAL_H
#define INTERNAL_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Size definitions */
#define TINY_SIZE 128
#define SMALL_SIZE 1024
#define MIN_ALLOCS_PER_ZONE 100

/* Alignment */
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

/* Defining min and max for all type of blocks */
#define MIN_TINY_SIZE    1
#define MAX_TINY_SIZE    ALIGN(TINY_SIZE)
#define MIN_SMALL_SIZE   MAX_TINY_SIZE + 1
#define MAX_SMALL_SIZE   ALIGN(SMALL_SIZE)
#define MIN_LARGE_SIZE   MAX_SMALL_SIZE + 1


typedef struct mem_block_header_s     mem_block_header_t;
typedef struct zone_header_s          zone_header_t;
typedef uint8_t*                      buff_t;

/*******************************/
/* MEMORY BLOCKS RELATED STUFF */
/*******************************/

typedef enum block_state_e
{
	BLOCK_STATE_EMPTY,
	BLOCK_STATE_FULL
} block_state_t;

typedef struct mem_block_header_s
{
	mem_block_header_t    *prev;
	mem_block_header_t    *next;
	zone_header_t         *belonging_zone;
	block_state_t          state;
	size_t                 requested_size;
	size_t                 user_data_size;
	void                  *user_data;
} mem_block_header_t;

typedef mem_block_header_t mem_block_t;

/* Defining Blocks MACROS */
#define BLOCK_HEADER_SIZE      ALIGN(sizeof(struct mem_block_header_s))
#define MAX_TINY_BLOCK_SIZE    ( BLOCK_HEADER_SIZE + MAX_TINY_SIZE )
#define MAX_SMALL_BLOCK_SIZE   ( BLOCK_HEADER_SIZE + MAX_SMALL_SIZE )


/******************************/
/* MEMORY ZONES RELATED STUFF */
/******************************/

typedef enum zone_type_e
{
	TINY,
	SMALL,
	LARGE
} zone_type_t;

typedef struct zone_header_s
{
	zone_header_t *prev;
	zone_header_t *next;
	mem_block_t   *first_block;
	mem_block_t   *last_block;
	size_t         zone_data_size;       /*!< the size used for the blocks inside */
	size_t         free_data_size;       /*!< the size free space for blocks inside the zone */
	size_t         total_size;           /*!< the total mmapped memory to be ummap*/
	size_t         allocations_count;
	zone_type_t    type;
} zone_header_t;

typedef zone_header_t          mem_zone_t;


#define ZONE_HEADER_SIZE ALIGN(sizeof(zone_header_t))

#define TINY_ZONE_SIZE    ( ZONE_HEADER_SIZE + ( MIN_ALLOCS_PER_ZONE * MAX_TINY_BLOCK_SIZE ) )
#define SMALL_ZONE_SIZE   ( ZONE_HEADER_SIZE + ( MIN_ALLOCS_PER_ZONE * MAX_SMALL_BLOCK_SIZE ) )

size_t get_page_size(void);
size_t make_page_size_multiple(size_t size);
void puthex_out(unsigned long n);
void dump_block(mem_block_t *block);
void dump_zone(mem_zone_t *zone);
void dump_memory();

/* HERE lies the globals */
typedef struct memory_zones_s
{
	mem_zone_t *tiny_zones;
	mem_zone_t *small_zones;
	mem_zone_t *large_zones;
} memory_zones_t;

void dump_globals();

#endif /* INTERNAL_H */
