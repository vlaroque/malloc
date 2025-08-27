/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/16 20:04:47 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 20:06:40 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_parsing.h"
#include "getdata_varg.h"

static int	bonus_star(t_parsedata *data, va_list *ap, int *h, int width)
{
	int		nbr;

	(*h)++;
	nbr = next_int(ap);
	if (nbr < 0)
	{
		if (width)
			data->flags = (data->flags | 1);
		nbr = -nbr;
	}
	return (nbr);
}

int			get_index(char *str, int *h)
{
	int		hsave;
	int		index;

	hsave = *h;
	index = mini_atoi(str, h);
	if (str[*h] == '$' && index >= 0)
	{
		(*h)++;
		return (index);
	}
	else if (str[*h] == '$')
	{
		(*h)++;
		return (-1);
	}
	*h = hsave;
	return (-1);
}

int			ft_conversion(char *str, int *h, va_list *ap, int fd)
{
	t_parsedata		data;
	int				(*f)(t_parsedata data, va_list *ap);

	(*h)++;
	data.fd = fd;
	data.flags = ft_flagparser(str, h);
	data.index = get_index(str, h);
	data.width = mini_atoi(str, h);
	if (str[*h] == '*')
		data.width = bonus_star(&data, ap, h, 1);
	data.precision = -1;
	if (str[*h] == '.')
	{
		data.precision = -42;
		if (ft_isdigit(str[++(*h)]))
			data.precision = mini_atoi(str, h);
		else if (str[*h] == '*')
			data.precision = bonus_star(&data, ap, h, 0);
	}
	data.size = ft_typeparser(str, h);
	if (!(f = ft_char_to_func(str, h)))
		return (0);
	return ((f)(data, ap));
}
