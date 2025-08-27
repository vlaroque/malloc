/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_color.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/08 14:52:05 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/18 18:22:23 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_color.h"
#include "ft_writings.h"

static int		ft_chckclr(char *str, char *color, int colorlen, int *h)
{
	int		i;

	i = 0;
	while (str[i] == color[i] && i < colorlen - 1)
		i++;
	if (i == colorlen - 1)
	{
		*h += colorlen;
		return (1);
	}
	return (0);
}

int				ft_colortxt(char *str, int *h, va_list *ap, int fd)
{
	(void) ap;

	if (ft_chckclr(&str[*h], "{red}", 5, h))
		write(fd, "\033[31m", 5);
	else if (ft_chckclr(&str[*h], "{green}", 7, h))
		write(fd, "\033[32m", 5);
	else if (ft_chckclr(&str[*h], "{yellow}", 8, h))
		write(fd, "\033[33m", 5);
	else if (ft_chckclr(&str[*h], "{blue}", 6, h))
		write(fd, "\033[34m", 5);
	else if (ft_chckclr(&str[*h], "{pink}", 6, h))
		write(fd, "\033[35m", 5);
	else if (ft_chckclr(&str[*h], "{cyan}", 6, h))
		write(fd, "\033[36m", 5);
	else if (ft_chckclr(&str[*h], "{grey}", 6, h))
		write(fd, "\033[37m", 5);
	else if (ft_chckclr(&str[*h], "{eoc}", 5, h))
		return (write(fd, "\033[0m", 4));
	else
	{
		write(fd, &str[*h], 1);
		*h = *h + 1;
		return (1);
	}
	return (5);
}
