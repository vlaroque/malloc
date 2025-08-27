/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_f_writings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 15:26:43 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 19:57:32 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "put_f.h"
#include "ft_writings.h"
#include "ft_meganbr.h"

int		put_spaces_or_zeros(t_parsedata data, int intlen, int state, int neg)
{
	int i;
	int len;

	if (data.flags & (1 << 5))
		intlen = intlen + (intlen / 3);
	len = pre_len(data.precision) + intlen + neg;
	i = -1;
	if (state == 0 && data.width > len && (data.flags & (1 << 2)))
	{
		while (++i < data.width - (len))
			ft_putchar_fd('0', data.fd);
	}
	else if (state == 2 && data.width > len
		&& !(data.flags & (1 << 2)) && data.flags & 1)
	{
		while (++i < data.width - (len))
			ft_putchar_fd(' ', data.fd);
	}
	else if (state == 1 && data.width > len
		&& !(data.flags & (1 << 2)) && !(data.flags & 1))
	{
		while (++i < data.width - (len))
			ft_putchar_fd(' ', data.fd);
	}
	return ((i == -1) ? 0 : i);
}

int		printsign(t_parsedata data, int neg)
{
	if (neg == 1)
	{
		ft_putchar_fd('-', data.fd);
		return (1);
	}
	else if (data.flags & (1 << 1))
	{
		ft_putchar_fd('+', data.fd);
		return (1);
	}
	else if (data.flags & (1 << 3))
	{
		ft_putchar_fd(' ', data.fd);
		return (1);
	}
	return (0);
}

int		print_intpart(int len, char *str, t_parsedata data)
{
	int i;

	i = 4950 - len;
	while (i < 4949)
	{
		ft_putchar_fd(str[i], data.fd);
		if (((4949 - i) % 3) == 0 && (data.flags & (1 << 5)))
		{
			ft_putchar_fd(',', data.fd);
			len++;
		}
		i++;
	}
	ft_putchar_fd(str[i], data.fd);
	return (len);
}

int		print_precision(t_parsedata data, char *str)
{
	int		i;
	int		prelen;

	i = 0;
	prelen = pre_len(data.precision);
	if (prelen)
	{
		ft_putchar_fd('.', data.fd);
		while (i < prelen - 1 && i + 4950 < STRNBRLEN)
		{
			ft_putchar_fd(str[i + 4950], data.fd);
			i++;
		}
		while (i++ < prelen - 1)
			ft_putchar_fd('0', data.fd);
	}
	return (prelen);
}
