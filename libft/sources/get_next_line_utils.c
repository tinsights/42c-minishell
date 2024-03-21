/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:07:18 by tjegades          #+#    #+#             */
/*   Updated: 2024/01/24 15:07:18 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	gnl_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != 0)
		i++;
	return (i);
}

char	*gnl_calloc(int n)
{
	char	*s;

	s = (char *)malloc(n);
	if (s == NULL)
		return (NULL);
	gnl_bzero(s, n);
	return (s);
}

void	gnl_bzero(char *s, int size)
{
	int	i;

	i = 0;
	while (i < size)
		s[i++] = 0;
}
