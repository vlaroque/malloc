/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_oxxp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 15:19:54 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/19 08:49:23 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include "ft_printf.h"
#include "ft_generic_int_fct.h"
#include "getdata_varg.h"
#include "ft_writings.h"

int			put_o(t_parsedata data, va_list *ap)
{
	uintmax_t	nbr;
	int			prefix;
	int			len;
	int			printedchars;

	printedchars = 0;
	data = flag_cleaner(data);
	nbr = getuint(data, ap);
	prefix = 0;
	if (nbr == 0 && (data.precision == 0 || data.precision == -42)
		&& !(data.flags & (1 << 4)))
		return (zero(data));
	if ((data.flags & (1 << 4)) && nbr)
		prefix = 1;
	len = mega_nbrlen_base_unsigned(nbr, "01234567");
	printedchars += put_spaces(printedchars, data, len, prefix);
	if (prefix)
		printedchars += ft_putchar_fd('0', data.fd);
	printedchars += put_zeros(data, len, prefix);
	mega_putnbr_base_unsigned(nbr, "01234567", data.fd);
	printedchars += len;
	printedchars += put_spaces(printedchars, data, len, prefix);
	return (printedchars);
}

int			put_x(t_parsedata data, va_list *ap)
{
	uintmax_t	nbr;
	int			prefix;
	int			len;
	int			printedchars;

	printedchars = 0;
	data = flag_cleaner(data);
	nbr = getuint(data, ap);
	prefix = 0;
	if (nbr == 0 && (data.precision == 0 || data.precision == -42))
		return (zero(data));
	if ((data.flags & (1 << 4)) && nbr)
		prefix = 2;
	len = mega_nbrlen_base_unsigned(nbr, "0123456789abcdef");
	printedchars += put_spaces(printedchars, data, len, prefix);
	if (prefix)
		printedchars += ft_putstr_fd("0x", data.fd);
	printedchars += put_zeros(data, len, prefix);
	mega_putnbr_base_unsigned(nbr, "0123456789abcdef", data.fd);
	printedchars += len;
	printedchars += put_spaces(printedchars, data, len, prefix);
	return (printedchars);
}

int			put_m_x(t_parsedata data, va_list *ap)
{
	uintmax_t	nbr;
	int			prefix;
	int			len;
	int			printedchars;

	printedchars = 0;
	data = flag_cleaner(data);
	nbr = getuint(data, ap);
	prefix = 0;
	if (nbr == 0 && (data.precision == 0 || data.precision == -42))
		return (zero(data));
	if ((data.flags & (1 << 4)) && nbr)
		prefix = 2;
	len = mega_nbrlen_base_unsigned(nbr, "0123456789ABCDEF");
	printedchars += put_spaces(printedchars, data, len, prefix);
	if (prefix)
		printedchars += ft_putstr_fd("0X", data.fd);
	printedchars += put_zeros(data, len, prefix);
	mega_putnbr_base_unsigned(nbr, "0123456789ABCDEF", data.fd);
	printedchars += len;
	printedchars += put_spaces(printedchars, data, len, prefix);
	return (printedchars);
}

int			put_p(t_parsedata data, va_list *ap)
{
	void		*ptr;
	int			prefix;
	int			len;
	int			printedchars;

	printedchars = 0;
	data = flag_cleaner(data);
	ptr = (void *)va_arg(*ap, void *);
	prefix = 2;
	if (ptr == NULL && (data.precision == 0 || data.precision == -42))
	{
		if (prefix)
			printedchars += ft_putstr_fd("0x", data.fd);
		return (zero(data) + printedchars);
	}
	len = mega_nbrlen_base_unsigned((uintmax_t)ptr, "0123456789abcdef");
	printedchars += put_spaces(printedchars, data, len, prefix);
	if (prefix)
		printedchars += ft_putstr_fd("0x", data.fd);
	printedchars += put_zeros(data, len, prefix);
	mega_putnbr_base_unsigned((uintmax_t)ptr, "0123456789abcdef", data.fd);
	printedchars += len;
	printedchars += put_spaces(printedchars, data, len, prefix);
	return (printedchars);
}
