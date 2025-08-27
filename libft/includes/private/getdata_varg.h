/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getdata_varg.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 14:44:59 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/08 05:58:41 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETINT_H
# define GETINT_H

#include <stdint.h>

uintmax_t		getuint(t_parsedata data, va_list *ap);
intmax_t		getint(t_parsedata data, va_list *ap);
long double		get_float(t_parsedata data, va_list *ap);
int				next_int(va_list *ap);

#endif
