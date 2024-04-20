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

bool valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}


bool valid_env_start(char *line)
{
	return (*line == '$' && (line[1] == '?' || valid_env_char(line[1])));
}

t_redir_type get_redir_type(char *line)
{
	int i = 0;

	if (!ft_strncmp(line, ">>", 2))
		return out_append;
	else if (!ft_strncmp(line, "<<", 2))
		return heredoc;
	else if (!ft_strncmp(line, ">", 1))
		return out_trunc;
	else if (!ft_strncmp(line, "<", 1))
		return input;
	else
		return (-1);
}

bool is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool is_single_pipe(char *line)
{
	return (line && line[0] == '|' && line[1] != '|');
}

int is_redirect(char *line)
{
	if (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2))
		return (2);
	else if (!ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1))
		return (1);
	else
		return (0);
}

bool is_meta(char *line)
{
	return (is_redirect(line) || !ft_strncmp(line, "|", 1));
}


