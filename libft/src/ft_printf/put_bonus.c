/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/08 07:26:26 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 17:55:00 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "ft_printf.h"
#include "ft_generic_int_fct.h"
#include "getdata_varg.h"
#include "ft_writings.h"

int			put_b(t_parsedata data, va_list *ap)
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
	len = mega_nbrlen_base_unsigned(nbr, "01");
	printedchars += put_spaces(printedchars, data, len, prefix);
	if (prefix)
		printedchars += ft_putstr_fd("0b", data.fd);
	printedchars += put_zeros(data, len, prefix);
	mega_putnbr_base_unsigned(nbr, "01", data.fd);
	printedchars += len;
	printedchars += put_spaces(printedchars, data, len, prefix);
	return (printedchars);
}

static int	print_non_printing(char c, int fd)
{
	if (c == '\0')
		return (write(fd, "^@", 2));
	else if (c == (char)9)
		return (write(fd, "\t", 1));
	else if (c == (char)10)
		return (write(fd, "$\n", 2));
	else if (c >= (char)1 && c <= (char)26)
	{
		write(fd, "^", 1);
		ft_putchar_fd(c + 'A' - 1, fd);
		return (2);
	}
	else if (c == (char)27)
		return (write(fd, "^[", 2));
	else if (c == (char)28)
		return (write(fd, "^\\", 2));
	else if (c == (char)29)
		return (write(fd, "^]", 2));
	else if (c == (char)30)
		return (write(fd, "^^", 2));
	else if (c == (char)31)
		return (write(fd, "^_", 2));
	else if (c == (char)127)
		return (write(fd, "^?", 2));
	return (0);
}

int			put_r(t_parsedata data, va_list *ap)
{
	int		i;
	int		printed;
	char	*str;

	i = 0;
	str = (char *)va_arg(*ap, void *);
	while (str[i])
	{
		if (str[i] <= 31 || str[i] == 127)
			printed += print_non_printing(str[i], data.fd);
		else
			printed += write(data.fd, str + i, 1);
		i++;
	}
	return (printed);
}
