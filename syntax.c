/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:01:36 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/23 12:01:36 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type	get_redir_type(char *line)
{
	int	i;

	i = 0;
	if (!ft_strncmp(line, ">>", 2))
		return (out_append);
	else if (!ft_strncmp(line, "<<", 2))
		return (heredoc);
	else if (!ft_strncmp(line, ">", 1))
		return (out_trunc);
	else if (!ft_strncmp(line, "<", 1))
		return (input);
	else
		return (-1);
}

bool	pipe_syntax_error(char *line, char *start)
{
	char	*ptr;

	ptr = line;
	if (ptr == start || !ptr[1])
		return (true);
	ptr--;
	while (is_space(*ptr))
	{
		if (ptr == start)
			return (true);
		ptr--;
	}
	if (*ptr == '|')
		return (true);
	ptr = line + 1;
	while (is_space(*ptr))
	{
		ptr++;
		if (!(*ptr))
			return (true);
	}
	if (*ptr == '|')
		return (true);
	return (false);
}

bool	redir_syntax_error(char **line_ptr)
{
	char	*line;

	if (!ft_strncmp((*line_ptr), "<<", 2) || !ft_strncmp((*line_ptr), ">>", 2))
		(*line_ptr)++;
	line = ++(*line_ptr);
	while (is_space(*line))
	{
		line++;
		if (!(*line))
			return (true);
	}
	if (!line[0] || is_redirect(line) || !ft_strncmp(line, "|", 1))
		return (true);
	return (false);
}

bool	invalid_syntax(char **line_ptr, int *cmd_count, char *start)
{
	char	*line;

	line = *line_ptr;
	if (*line == '\'' || *line == '"')
	{
		if (!(line[1]))
			return (true);
		*line_ptr = ft_strchr(line + 1, *line);
		if (!(*line_ptr))
			return (true);
	}
	else if (*line == '|')
	{
		if (pipe_syntax_error(line, start))
			return (true);
		(*cmd_count)++;
	}
	else if (is_redirect(line))
		if (redir_syntax_error(line_ptr))
			return (true);
	return (false);
}

int	count_cmds(char *line)
{
	int		cmd_count;
	char	*start;
	char	*ptr;

	while (is_space(*line))
		line++;
	if (!line[0])
		return (0);
	cmd_count = 1;
	start = line;
	while (*line)
	{
		if (invalid_syntax(&line, &cmd_count, start))
		{
			ft_putstr_fd("Syntax error\n", 2);
			return (-1);
		}
		else if (*line)
			line++;
	}
	return (cmd_count);
}
