/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_up_case.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 12:19:52 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 16:00:31 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_puts.h"

int		put_m_d(t_parsedata data, va_list *ap)
{
	data.size = 3;
	return (put_di(data, ap));
}

int		put_m_i(t_parsedata data, va_list *ap)
{
	data.size = 3;
	return (put_di(data, ap));
}

int		put_m_o(t_parsedata data, va_list *ap)
{
	data.size = 3;
	return (put_o(data, ap));
}

int		put_m_u(t_parsedata data, va_list *ap)
{
	data.size = 3;
	return (put_u(data, ap));
}

int		put_m_f(t_parsedata data, va_list *ap)
{
	data.size = 6;
	return (put_f(data, ap));
}
