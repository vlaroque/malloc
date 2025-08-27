/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puts.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 08:41:35 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/19 08:51:49 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PUTS_H
# define FT_PUTS_H
# include "ft_printf.h"

int				put_pc(t_parsedata data, va_list *ap);
int				put_c(t_parsedata data, va_list *ap);
int				put_s(t_parsedata data, va_list *ap);
int				put_di(t_parsedata data, va_list *ap);
int				put_u(t_parsedata data, va_list *ap);
int				put_o(t_parsedata data, va_list *ap);
int				put_x(t_parsedata data, va_list *ap);
int				put_m_x(t_parsedata data, va_list *ap);
int				put_p(t_parsedata data, va_list *ap);
int				put_f(t_parsedata data, va_list *ap);
int				put_b(t_parsedata data, va_list *ap);
int				put_r(t_parsedata data, va_list *ap);

int				put_m_d(t_parsedata data, va_list *ap);
int				put_m_i(t_parsedata data, va_list *ap);
int				put_m_o(t_parsedata data, va_list *ap);
int				put_m_u(t_parsedata data, va_list *ap);
int				put_m_f(t_parsedata data, va_list *ap);

#endif