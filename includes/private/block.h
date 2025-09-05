#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>
#include "internal.h"

/*!
 * \brief initiate a new empty block at location
 *
 * \param[in] zone is the pointer to the zone where the block belongs to
 * \param[in] block_to_init is the pointer to where initiate the block
 * \param[in] previous_block is the pointer to previous block
 * \param[in] next_block is the pointer to next block
 * \param[in] requested_size the size the user really requested
 * \param[in] user_data_size is the size of the data of the block, not the entire size of it
 * \param[in] state is the state of the block, if it contains data or not
 * 
 * \return the pointer to the user data, NULL if error occurs
 */
void *init_block(mem_zone_t    *zone,
                 mem_block_t   *block_to_init,
                 mem_block_t   *previous_block,
                 mem_block_t   *next_block,
                 size_t         requested_size,
                 size_t         user_data_size,
                 block_state_t  state);


mem_block_t *find_in_zone_block_big_enough(mem_zone_t *zone, size_t block_data_size);

void *occupy_empty_block(mem_zone_t *zone, mem_block_t *empty_block, size_t requested_size, size_t block_data_size);

void block_deallocate(mem_block_t *block);


#endif /* BLOCK_H */
