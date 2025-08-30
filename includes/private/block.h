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
 * \param[in] user_data_size is the size of the data of the block, not the entire size of it
 * \param[in] state is the state of the block, if it contains data or not
 * 
 * \return the pointer to the user data, NULL if error occurs
 */
void *init_block(mem_zone_t     zone,
                 mem_block_t   *block_to_init,
                 mem_block_t   *previous_block,
                 mem_block_t   *next_block,
                 size_t         user_data_size,
                 block_state_t  state);

/*!
 * \brief initiate a new empty block at location
 *
 * \param[in] zone the zone where the block is allocated
 * \param[in] location is the pointer to where initiate de empty block
 * \param[in] data_size is the size of the data of the block, not the entire size of it
 */
void first_empty_block(mem_zone_t zone, mem_block_t *block_to_init, size_t data_size);

void *try_allocate_in_zone(mem_zone_t zone, size_t block_data_size);


#endif /* BLOCK_H */
