/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_meganbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 15:31:20 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 17:36:47 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_meganbr.h"
#include "ft_meganbr_maths.h"
#include "ft_meganbr_processing.h"

static long double	two_exp(int exp)
{
	long double		res;

	res = 1.0;
	if (exp == 0)
		return ((long double)1.0);
	else if (exp == 1)
		return ((long double)2.0);
	else if (exp < 0)
		while (exp++ < 0)
			res /= 2.0;
	else if (exp > 0)
		while (exp-- > 0)
			res *= 2.0;
	return (res);
}

static int			get_exp(long double nbr)
{
	int		pwr;

	pwr = 0;
	if (nbr == 0.0)
		return (0);
	if (nbr >= 1)
	{
		while (nbr >= 1)
		{
			nbr /= 2.0;
			pwr += 1;
		}
		pwr -= 1;
	}
	else if (nbr < 1)
	{
		while (nbr < 1)
		{
			nbr *= 2.0;
			pwr -= 1;
		}
	}
	return (pwr);
}

static int			sign_ldbl(long double *nbr)
{
	int sign;

	if (*nbr < 0)
	{
		*nbr = -(*nbr);
		sign = -1;
	}
	return (sign);
}

static int			ft_ldbl_mantissa(long double nbr, t_ullong *meganbr)
{
	t_ullong	*added;

	added = meganbr_two_exp(1);
	while (nbr > 0.0)
	{
		added = meganbr_dividebytwo(added);
		if (nbr >= 1.0)
		{
			meganbr = meganbr_add(meganbr, added);
			nbr = nbr - 1.0;
		}
		nbr *= 2.0;
	}
	free(added);
	return (0);
}

char				*ft_ldbl_2_str(long double nbr)
{
	int			expt;
	int			sign;
	t_ullong	*meganbr;
	char		*resstr;

	if (!(meganbr = (t_ullong *)malloc(sizeof(t_ullong) * MEGALEN + 1)))
		return (NULL);
	meganbr_zero(meganbr);
	sign = sign_ldbl(&nbr);
	expt = get_exp(nbr);
	nbr /= two_exp(expt);
	ft_ldbl_mantissa(nbr, meganbr);
	meganbr = meganbr_exp(meganbr, expt);
	resstr = meganbr_be_str(meganbr);
	if (sign == -1)
		resstr[0] = '-';
	free(meganbr);
	return (resstr);
}
