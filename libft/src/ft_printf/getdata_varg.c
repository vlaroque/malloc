/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getdata_varg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 14:43:52 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 17:56:08 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdint.h>

uintmax_t	getuint(t_parsedata data, va_list *ap)
{
	uintmax_t res;

	if (data.size == 0)
		res = (uintmax_t)(unsigned int)va_arg(*ap, int);
	else if (data.size == 1)
		res = (uintmax_t)(unsigned char)va_arg(*ap, int);
	else if (data.size == 2)
		res = (uintmax_t)(unsigned short int)va_arg(*ap, int);
	else if (data.size == 3)
		res = (uintmax_t)va_arg(*ap, unsigned long int);
	else if (data.size == 4)
		res = (uintmax_t)va_arg(*ap, unsigned long long int);
	else if (data.size == 6)
		res = (uintmax_t)va_arg(*ap, uintmax_t);
	else if (data.size == 7)
		res = (uintmax_t)va_arg(*ap, ssize_t);
	return (res);
}

intmax_t	getint(t_parsedata data, va_list *ap)
{
	intmax_t res;

	if (data.size == 0)
		res = (intmax_t)(int)va_arg(*ap, int);
	else if (data.size == 1)
		res = (intmax_t)(char)va_arg(*ap, int);
	else if (data.size == 2)
		res = (intmax_t)(short int)va_arg(*ap, int);
	else if (data.size == 3)
		res = (intmax_t)va_arg(*ap, long int);
	else if (data.size == 4)
		res = (intmax_t)va_arg(*ap, long long int);
	else if (data.size == 6)
		res = (intmax_t)va_arg(*ap, intmax_t);
	else if (data.size == 7)
		res = (intmax_t)va_arg(*ap, size_t);
	return (res);
}

long double	get_float(t_parsedata data, va_list *ap)
{
	long double res;

	if (data.size == 0)
		res = (long double)va_arg(*ap, double);
	else if (data.size == 3)
		res = (long double)va_arg(*ap, double);
	else if (data.size == 5)
		res = (long double)va_arg(*ap, long double);
	return (res);
}

int			next_int(va_list *ap)
{
	return ((int)va_arg(*ap, int));
}
