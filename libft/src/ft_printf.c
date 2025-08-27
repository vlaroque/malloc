/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/16 14:46:58 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 17:57:12 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_writings.h"

int		char_analyser(char *str, int *h, va_list *ap, int fd)
{
	int				i;
	t_spe_chars		*tab_spe_chars;

	tab_spe_chars = init_spe_chars();
	i = 0;
	while (i < NB_SPE_CHARS && str[*h] != tab_spe_chars[i].c)
		i++;
	if (i < NB_SPE_CHARS && str[*h] == tab_spe_chars[i].c)
		return (tab_spe_chars[i].f(str, h, ap, fd));
	else if (str[*h])
	{
		ft_putchar_fd(str[*h], fd);
		(*h)++;
		return (1);
	}
	return (0);
}

int		ft_printf(char *str, ...)
{
	int			h;
	int			char_count;
	va_list		ap;

	h = 0;
	char_count = 0;
	va_start(ap, str);
	while (str[h])
		char_count += char_analyser(str, &h, &ap, 1);
	va_end(ap);
	return (char_count);
}

int		ft_fprintf(int fd, char *str, ...)
{
	int			h;
	int			char_count;
	va_list		ap;

	h = 0;
	char_count = 0;
	va_start(ap, str);
	while (str[h])
		char_count += char_analyser(str, &h, &ap, fd);
	va_end(ap);
	return (char_count);
}
