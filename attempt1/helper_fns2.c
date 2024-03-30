/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_fns2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 18:40:01 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 18:42:45 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
		ptr[i++] = c;
}

int	my_strcmp(char *delimiter, char *str)
{
	int	flag;

	flag = 0;
	while (*str && *delimiter)
	{
		if (*str++ != *delimiter++)
		{
			flag = 1;
			break ;
		}
	}
	if (!flag && *str == '\n')
		return (1);
	return (0);
}

int	is_alphabet(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'z'))
		return (1);
	return (0);
}

int	is_numeric(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
