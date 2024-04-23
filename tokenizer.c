/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 11:51:54 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/23 11:51:57 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_key(char *line);

int	env_len(char **line_ptr, char qstart, int hd_flag)
{
	char	*var;
	int		key_len;
	int		len;
	char	*value;

	var = get_env_key(*line_ptr);
	key_len = 0;
	len = 0;
	value = getenv(var);
	if (value)
		len = ft_strlen(value);
	key_len = ft_strlen(var) + 1;
	*line_ptr += key_len;
	free_str(&var);
	return (len + len_to_alloc(line_ptr, qstart, hd_flag));
}

int	len_to_alloc(char **line_ptr, char qstart, int hd_flag)
{
	if (!**line_ptr || (!qstart && (is_redirect(*line_ptr)
				|| is_space(**line_ptr))))
		return (0);
	if (qstart && **line_ptr == qstart)
	{
		(*line_ptr)++;
		qstart = 0;
	}
	if (!qstart && (!**line_ptr || is_redirect(*line_ptr)
			|| is_space(**line_ptr)))
		return (1);
	else if (!qstart && (**line_ptr == '\'' || **line_ptr == '"'))
	{
		qstart = **line_ptr;
		(*line_ptr)++;
		return (len_to_alloc(line_ptr, qstart, hd_flag));
	}
	else if ((hd_flag == 2 || qstart != '\'') && valid_env_start(*line_ptr)
		&& hd_flag)
		return (env_len(line_ptr, qstart, hd_flag));
	else
	{
		(*line_ptr)++;
		return (1 + len_to_alloc(line_ptr, qstart, hd_flag));
	}
}

bool	handle_env_copy(char **line_ptr, char qstart, char *word, int hd_flag)
{
	char	*var;
	int		key_len;
	int		len;
	char	*value;

	var = get_env_key(*line_ptr);
	key_len = 0;
	len = 0;
	value = getenv(var);
	if (value)
	{
		len = ft_strlen(value);
		ft_strlcpy(word, value, len + 1);
		word += len;
	}
	key_len = ft_strlen(var) + 1;
	*line_ptr += key_len;
	free_str(&var);
	return (word_copy(line_ptr, qstart, word, hd_flag));
}

bool	end_of_line(char **line_ptr, char *qstart)
{
	if (!**line_ptr)
		return (true);
	if (*qstart && **line_ptr == *qstart)
	{
		(*line_ptr)++;
		*qstart = 0;
	}
	return (!*qstart && (!**line_ptr || is_redirect(*line_ptr)
			|| is_space(**line_ptr)));
}

bool	word_copy(char **ptr, char qstart, char *word, int flag)
{
	static bool	quoted;
	bool		result;

	if (end_of_line(ptr, &qstart))
	{
		result = quoted;
		quoted = false;
		return (result);
	}
	else if (!qstart && (**ptr == '\'' || **ptr == '"'))
	{
		quoted = true;
		qstart = **ptr;
		(*ptr)++;
		return (word_copy(ptr, qstart, word, flag));
	}
	else if ((flag == 2 || qstart != '\'') && valid_env_start(*ptr) && flag)
		return (handle_env_copy(ptr, qstart, word, flag));
	else
	{
		*word = **ptr;
		(*ptr)++;
		word++;
		return (word_copy(ptr, qstart, word, flag));
	}
}
