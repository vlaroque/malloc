/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 21:05:38 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 16:12:11 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_twocharscmp(char *str, char char1, char char2)
{
	if (str[0] && str[0] == char1)
	{
		if (char2)
		{
			if (str[1] == char2)
				return (1);
			return (0);
		}
		return (1);
	}
	return (0);
}

void	*ft_char_to_func(char *str, int *h)
{
	int				i;
	t_conv_chars	*tab_conv_chars;

	i = 0;
	tab_conv_chars = init_conv_chars();
	while (i < NB_CONV_CHARS && str[*h] && str[*h] != tab_conv_chars[i].c)
		i++;
	if (str[*h] == tab_conv_chars[i].c)
	{
		(*h)++;
		return (tab_conv_chars[i].f);
	}
	return (NULL);
}

char	ft_typeparser(char *str, int *h)
{
	char res;

	res = 0;
	if (ft_twocharscmp(str + (*h), 'h', 'h'))
		res = 1;
	else if (ft_twocharscmp(str + (*h), 'l', 'l'))
		res = 4;
	else if (ft_twocharscmp(str + (*h), 'h', '\0'))
		res = 2;
	else if (ft_twocharscmp(str + (*h), 'l', '\0'))
		res = 3;
	else if (ft_twocharscmp(str + (*h), 'L', '\0'))
		res = 5;
	else if (ft_twocharscmp(str + (*h), 'j', '\0'))
		res = 6;
	else if (ft_twocharscmp(str + (*h), 'z', '\0'))
		res = 7;
	else
		return (0);
	if (res == 1 || res == 4)
		(*h) += 2;
	else
		(*h)++;
	return (res);
}

int		mini_atoi(char *str, int *h)
{
	int	res;

	res = 0;
	if (!ft_isdigit(str[*h]))
		return (-1);
	while (ft_isdigit(str[*h]))
	{
		res = res * 10 + (str[*h] - '0');
		(*h)++;
	}
	return (res);
}

char	ft_flagparser(char *str, int *h)
{
	char	flags;

	flags = '\0';
	while (1)
	{
		if (str[*h] == '\0')
			return (-1);
		else if (str[*h] == '-')
			flags = (flags | 1);
		else if (str[*h] == '+')
			flags = (flags | (1 << 1));
		else if (str[*h] == '0')
			flags = (flags | (1 << 2));
		else if (str[*h] == ' ')
			flags = (flags | (1 << 3));
		else if (str[*h] == '#')
			flags = (flags | (1 << 4));
		else if (str[*h] == '\'')
			flags = (flags | (1 << 5));
		else
			break ;
		(*h)++;
	}
	return (flags);
}
