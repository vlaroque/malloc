/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_meganbr_processing.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 11:50:55 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 17:37:28 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_meganbr.h"
#include <stdio.h>

int		uullong_to_str(char *str, t_ullong nbr)
{
	str = str + 17;
	while (nbr > 0)
	{
		*str = (nbr % 10) + '0';
		nbr = nbr / 10;
		str--;
	}
	return (1);
}

int		ullong_to_str(char *str, int start, t_ullong nbr)
{
	int i;

	i = 17 + start;
	while (nbr > (t_ullong)0 && i >= 0)
	{
		str[i] = (nbr % 10) + '0';
		nbr = nbr / 10;
		i--;
	}
	return (1);
}

char	*meganbr_be_str(t_ullong *meganbr)
{
	int		i;
	int		j;
	char	*strnbr;

	i = 0;
	j = 0;
	if (!(strnbr = malloc(sizeof(char) * (STRNBRLEN + 1))))
		return (NULL);
	while (j < STRNBRLEN)
		strnbr[j++] = '0';
	while (i < MEGALEN)
	{
		ullong_to_str(strnbr, (i * 18), meganbr[i]);
		i++;
	}
	strnbr[STRNBRLEN] = '\0';
	return (strnbr);
}

void	str_to_zero(char *str)
{
	int i;

	i = 0;
	while (i <= STRNBRLEN)
	{
		str[i] = 0;
		i++;
	}
}

int		meganbr_zero(t_ullong *meganbr)
{
	int i;

	i = 0;
	while (i < MEGALEN)
	{
		meganbr[i] = 0;
		i++;
	}
	return (0);
}
