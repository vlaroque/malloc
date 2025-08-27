/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_data_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 16:55:42 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 16:30:39 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_data_init.h"
#include "ft_puts.h"

t_spe_chars			*init_spe_chars(void)
{
	static t_spe_chars	*spe_chars_tab = NULL;

	if (!(spe_chars_tab))
	{
		if (!(spe_chars_tab = malloc(sizeof(t_spe_chars) * NB_SPE_CHARS)))
			return (0);
		spe_chars_tab[0].c = '%';
		spe_chars_tab[0].f = ft_conversion;
		spe_chars_tab[1].c = '{';
		spe_chars_tab[1].f = ft_colortxt;
	}
	return (spe_chars_tab);
}

static t_conv_chars	*init_conv_chars_filler(t_conv_chars *conv_chars_tab)
{
	conv_chars_tab[0].c = '%';
	conv_chars_tab[0].f = put_pc;
	conv_chars_tab[1].c = 'c';
	conv_chars_tab[1].f = put_c;
	conv_chars_tab[2].c = 's';
	conv_chars_tab[2].f = put_s;
	conv_chars_tab[3].c = 'd';
	conv_chars_tab[3].f = put_di;
	conv_chars_tab[4].c = 'i';
	conv_chars_tab[4].f = put_di;
	conv_chars_tab[5].c = 'u';
	conv_chars_tab[5].f = put_u;
	conv_chars_tab[6].c = 'o';
	conv_chars_tab[6].f = put_o;
	conv_chars_tab[7].c = 'x';
	conv_chars_tab[7].f = put_x;
	conv_chars_tab[8].c = 'X';
	conv_chars_tab[8].f = put_m_x;
	conv_chars_tab[9].c = 'p';
	conv_chars_tab[9].f = put_p;
	conv_chars_tab[10].c = 'f';
	conv_chars_tab[10].f = put_f;
	return (conv_chars_tab);
}

static t_conv_chars	*init_conv_chars_filler_bonus(t_conv_chars *conv_chars_tab)
{
	conv_chars_tab[11].c = 'b';
	conv_chars_tab[11].f = put_b;
	conv_chars_tab[12].c = 'r';
	conv_chars_tab[12].f = put_r;
	conv_chars_tab[13].c = 'D';
	conv_chars_tab[13].f = put_m_d;
	conv_chars_tab[14].c = 'I';
	conv_chars_tab[14].f = put_m_i;
	conv_chars_tab[15].c = 'O';
	conv_chars_tab[15].f = put_m_o;
	conv_chars_tab[16].c = 'U';
	conv_chars_tab[16].f = put_m_u;
	conv_chars_tab[17].c = 'F';
	conv_chars_tab[17].f = put_m_f;
	return (conv_chars_tab);
}

t_conv_chars		*init_conv_chars(void)
{
	static t_conv_chars	*tab = NULL;

	if (!(tab))
	{
		if (!(tab = (t_conv_chars *)malloc(sizeof(*tab) * NB_CONV_CHARS)))
			return (0);
		tab = init_conv_chars_filler(tab);
		tab = init_conv_chars_filler_bonus(tab);
	}
	return (tab);
}
