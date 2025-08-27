/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putnbr_base.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 14:43:08 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 14:32:51 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "ft_printf.h"
#include "ft_writings.h"

void		mega_putnbr_base_unsigned(uintmax_t nbr, char *base, int fd)
{
	int		basesize;

	basesize = ft_strlen(base);
	if (nbr >= (uintmax_t)basesize)
	{
		mega_putnbr_base_unsigned(nbr / basesize, base, fd);
		ft_putchar_fd(base[nbr % basesize], fd);
	}
	else
		ft_putchar_fd(base[nbr % basesize], fd);
}

uintmax_t	ft_pow(uintmax_t nbr, uintmax_t pwr)
{
	uintmax_t res;

	res = 1;
	while (pwr > 0)
	{
		res = res * nbr;
		pwr--;
	}
	return (res);
}

int			put_unbr(uintmax_t nbr, int quote, int fd)
{
	int			len;
	int			i;
	uintmax_t	denom;
	uintmax_t	save;

	save = nbr;
	len = 0;
	if (nbr == 0)
		return (ft_putchar_fd('0', fd));
	while (nbr && ++len)
		nbr /= 10;
	i = len;
	denom = ft_pow(10, len - 1);
	while (denom)
	{
		ft_putchar_fd(((save / denom) % 10) + '0', fd);
		if (quote && i % 3 == 1 && denom > 10 && ++len)
			ft_putchar_fd(',', fd);
		denom /= 10;
		i--;
	}
	return (len);
}

int			unbr_len(uintmax_t nbr, int quote)
{
	int	len;

	len = 0;
	if (nbr == 0)
		return (1);
	while (nbr)
	{
		len++;
		nbr /= 10;
	}
	if (quote)
		return (len + (len / 3));
	return (len);
}
