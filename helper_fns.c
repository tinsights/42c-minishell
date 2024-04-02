/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_fns.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 11:27:27 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 12:50:52 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	my_strlen(char *s)
{
	int	len;

	len = 0;
	if (s && *s)
		while (*s++)
			len++;
	return (len);
}

int	my_strncmp(char *s1, char *s2, size_t n)
{
	unsigned char	*c1;
	unsigned char	*c2;

	c1 = (unsigned char *)s1;
	c2 = (unsigned char *)s2;
	while ((*c1 || *c2) && n)
	{
		if (*c1 != *c2)
			return (*c1 - *c2);
		c1++;
		c2++;
		n--;
	}
	if (!*c2 && !n)
		return (0);
	return (1);
}

void	free_str_nbr(char *s1, char *s2, int str_nbr)
{
	if (str_nbr == 1)
		free(s1);
	else if (str_nbr == 2)
		free(s2);
}

char	*strjoin_and_free_str(char *s1, char *s2, int str_nbr)
{
	char	*str;
	int		str_len;
	int		i;
	int		j;

	i = 0;
	j = -1;
	str_len = my_strlen(s1) + my_strlen(s2);
	str = (char *)malloc(sizeof(char) * (str_len + 1));
	if (!str)
		return (NULL);
	str[str_len] = '\0';
	if (s1 != NULL)
	{
		while (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
	}
	while (s2[++j])
		str[i++] = s2[j];
	free_str_nbr(s1, s2, str_nbr);
	return (str);
}

int	is_whitespace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}
