/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_f_rounding.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 15:34:41 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/23 17:39:07 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "put_f.h"

int		pre_len(int precision)
{
	int len;

	len = 0;
	if (precision == -42 || precision == 0)
		len = 0;
	else if (precision == -1)
		len = 7;
	else
		len = precision + 1;
	return (len);
}

int		int_len(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] == '0')
		i++;
	if (i == 0)
		return (1);
	if (i < 4949)
		return (4950 - i);
	return (1);
}

int		round_check(char *str, int i)
{
	i++;
	if (str[i] - '0' < 5)
		return (0);
	else if (str[i] - '0' == 5)
	{
		i++;
		while (str[i] == '0' && i < 21499)
			i++;
		if (str[i] - '0' != 0)
			return (1);
	}
	else if (str[i] - '0' > 5)
		return (1);
	return (0);
}

int		rounding(t_parsedata data, char *str)
{
	int i;
	int carry;
	int tmp;

	carry = 0;
	if (pre_len(data.precision) == 0 && round_check(str, 4949))
	{
		i = 4949;
		carry = 1;
	}
	else if (data.precision > 0 && round_check(str, 4949 + data.precision))
	{
		i = 4949 + data.precision;
		carry = 1;
	}
	while (carry)
	{
		tmp = str[i] + carry - '0';
		str[i] = (tmp % 10) + '0';
		carry = tmp / 10;
		i--;
	}
	return (1);
}
