/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_meganbr.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 11:16:04 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 15:59:52 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MEGANBR_H
# define FT_MEGANBR_H

# include <stdlib.h>
# define MEGALEN 1189
# define MEGAUNIT 274
//# define STRNBRLEN 21380
# define STRNBRLEN 21450

typedef	unsigned long long	t_ullong;
char	*ft_ldbl_2_str(long double nbr);
void	str_to_zero(char *str);

#endif
