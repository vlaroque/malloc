/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_meganbr_maths.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 19:19:15 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 16:17:52 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_meganbr.h"
#include "ft_meganbr_processing.h"

t_ullong	*meganbr_multiplybytwo(t_ullong *meganbr)
{
	int			i;
	t_ullong	carry;
	t_ullong	tmp;

	i = MEGALEN - 1;
	carry = 0;
	while (i >= 0)
	{
		tmp = ((meganbr[i] * 2) + carry);
		meganbr[i] = tmp % 1000000000000000000;
		carry = tmp / 1000000000000000000;
		i--;
	}
	return (meganbr);
}

t_ullong	*meganbr_dividebytwo(t_ullong *meganbr)
{
	int			i;
	t_ullong	tmp;
	t_ullong	carry;

	i = 0;
	carry = 0;
	while (i < MEGALEN)
	{
		tmp = meganbr[i] + (carry * 1000000000000000000);
		meganbr[i] = tmp / 2;
		carry = tmp % 2;
		i++;
	}
	return (meganbr);
}

t_ullong	*meganbr_two_exp(int exp)
{
	t_ullong	*meganbr;

	if (!(meganbr = malloc(sizeof(t_ullong) * MEGALEN)))
		return (NULL);
	meganbr_zero(meganbr);
	meganbr[MEGAUNIT] = 1;
	if (exp > 0)
	{
		while (exp > 0)
		{
			meganbr = meganbr_multiplybytwo(meganbr);
			exp--;
		}
	}
	else if (exp < 0)
	{
		while (exp < 0)
		{
			meganbr = meganbr_dividebytwo(meganbr);
			exp++;
		}
	}
	return (meganbr);
}

t_ullong	*meganbr_exp(t_ullong *meganbr, int exp)
{
	if (exp > 0)
	{
		while (exp > 0)
		{
			meganbr = meganbr_multiplybytwo(meganbr);
			exp--;
		}
	}
	else if (exp < 0)
	{
		while (exp < 0)
		{
			meganbr = meganbr_dividebytwo(meganbr);
			exp++;
		}
	}
	return (meganbr);
}

t_ullong	*meganbr_add(t_ullong *meganbr, t_ullong *added)
{
	int			i;
	t_ullong	carry;
	t_ullong	save;

	i = MEGALEN - 1;
	carry = 0;
	while (i >= 0)
	{
		save = meganbr[i] + added[i] + carry;
		meganbr[i] = save % 1000000000000000000;
		carry = save / 1000000000000000000;
		i--;
	}
	return (meganbr);
}
