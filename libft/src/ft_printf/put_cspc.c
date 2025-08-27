/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_cspc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 13:56:27 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 16:06:36 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"
#include "ft_writings.h"
#include "ft_generic_int_fct.h"

static int	put_spaces_csppc(t_parsedata data, int len, int printed, char end)
{
	int	res;

	res = 0;
	if (data.width > len + printed && !(data.flags & 1))
	{
		while (data.width > len + printed + res)
		{
			ft_putchar_fd(' ', data.fd);
			res++;
		}
	}
	else if (data.width > len + printed && (data.flags & 1) && end)
	{
		while (data.width > printed + res)
		{
			ft_putchar_fd(' ', data.fd);
			res++;
		}
	}
	return (res);
}

int			put_pc(t_parsedata data, va_list *ap)
{
	int		printedchars;
	int		len;

	(void) ap;
	printedchars = 0;
	len = 1;
	printedchars += put_spaces_csppc(data, len, printedchars, 0);
	printedchars += ft_putstr_fd("%", data.fd);
	printedchars += put_spaces_csppc(data, len, printedchars, 1);
	return (printedchars);
}

int			put_s(t_parsedata data, va_list *ap)
{
	char	*str;
	int		printedchars;
	int		len;

	printedchars = 0;
	str = ft_strdup((char *)va_arg(*ap, void *));
	if (data.precision == -42)
		data.precision = 0;
	if (str)
	{
		if (data.precision >= 0 && data.precision < ft_strlen(str))
			str[data.precision] = '\0';
		len = ft_strlen(str);
	}
	else
		len = 6;
	printedchars += put_spaces_csppc(data, len, printedchars, 0);
	if (str)
		printedchars += ft_putstr_fd(str, data.fd);
	else
		printedchars += ft_putstr_fd("(null)", data.fd);
	printedchars += put_spaces_csppc(data, len, printedchars, 1);
	if (str)
		free(str);
	return (printedchars);
}

int			put_c(t_parsedata data, va_list *ap)
{
	char	c;
	char	str[2];
	int		printedchars;
	int		len;

	printedchars = 0;
	c = (char)va_arg(*ap, int);
	if (c)
	{
		str[0] = c;
		str[1] = '\0';
	}
	len = 1;
	printedchars += put_spaces_csppc(data, len, printedchars, 0);
	if (c)
		printedchars += ft_putstr_fd(str, data.fd);
	else
	{
		printedchars += 1;
		ft_putchar_fd('\0', data.fd);
	}
	printedchars += put_spaces_csppc(data, len, printedchars, 1);
	return (printedchars);
}
