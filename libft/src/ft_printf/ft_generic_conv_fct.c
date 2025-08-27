/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_generic_conv_fct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 14:28:31 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 20:03:03 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_writings.h"

t_parsedata	flag_cleaner(t_parsedata data)
{
	if ((data.flags & (1 << 2)) && (data.flags & 1))
		data.flags -= (1 << 2);
	if (data.flags & (1 << 2) && (data.precision >= 0 || data.precision == -42))
		data.flags -= (1 << 2);
	return (data);
}

int			print_char_x_times(char c, int times, int fd)
{
	int res;

	res = times;
	if (times < 0)
		return (0);
	while (times--)
		ft_putchar_fd(c, fd);
	return (res);
}