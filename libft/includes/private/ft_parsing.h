/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 21:38:12 by vlaroque          #+#    #+#             */
/*   Updated: 2019/01/18 09:43:55 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PARSING_H
# define FT_PARSING_H

char	ft_typeparser(char *str, int *h);
int		mini_atoi(char *str, int *h);
char	ft_flagparser(char *str, int *h);
void	*ft_char_to_func(char *str, int *h);

# endif