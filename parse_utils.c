/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:26:32 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/20 11:26:34 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	valid_env_start(char *line)
{
	return (*line == '$' && (line[1] == '?' || valid_env_char(line[1])));
}

bool	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_redirect(char *line)
{
	if (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2))
		return (2);
	else if (!ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1))
		return (1);
	else
		return (0);
}

char	*get_env_key(char *line)
{
	char	*result;
	int		i;

	i = 1;
	if (line[i] == '?')
		return (ft_substr(line, 1, 1));
	if (line[i] == '_' || ft_isalpha(line[i]))
	{
		i++;
		while (valid_env_char(line[i]))
			i++;
		result = ft_substr(line, 1, i - 1);
		return (result);
	}
	return (NULL);
}
