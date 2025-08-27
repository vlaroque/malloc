/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_writings.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaroque <vlaroque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 15:20:41 by vlaroque          #+#    #+#             */
/*   Updated: 2019/03/20 17:56:47 by vlaroque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		ft_strlen(const char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int		ft_putchar_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}

int		ft_putstr_fd(char *str, int fd)
{
	int		len;

	if (str == NULL)
		return (0);
	len = ft_strlen(str);
	write(fd, str, len);
	return (len);
}
