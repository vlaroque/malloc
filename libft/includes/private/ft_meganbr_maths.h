/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_meganbr_maths.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 19:20:51 by vlaroque          #+#    #+#             */
/*   Updated: 2019/02/28 09:50:17 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MEGANBR_MATHS_H
# define FT_MEGANBR_MATHS_H

# include "ft_meganbr.h"

t_ullong	*meganbr_multiplybytwo(t_ullong *meganbr);
t_ullong	*meganbr_dividebytwo(t_ullong *meganbr);
t_ullong	*meganbr_two_exp(int exp);
t_ullong	*meganbr_exp(t_ullong *meganbr, int exp);
t_ullong	*meganbr_add(t_ullong *meganbr, t_ullong *added);

#endif
